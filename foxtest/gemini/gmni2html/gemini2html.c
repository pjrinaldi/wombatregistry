/*
    gemini2html  Converts Gemini to HTML
    Copyright (C) 2022  Nicholas Johnson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gemini2html.h"

/* Escapes text so it can be safely inserted within HTML tags and attributes.
 * 
 * Examples:
 *   escape_text("5 < 6", 5)            => "5 &lt; 6"
 *   escape_text("6 > 5", 5)            => "6 &gt; 5"
 *   escape_text("me & you", 8)         => "me &amp; you"
 *   escape_text("i'm so high", 11)     => "I&apos;m so high"
 *   escape_text("\"hello world\"", 13) => "&quot;hello world&quot;"
 */
char* escape_text(const char* unescaped, const size_t unescaped_len) {
    /* In the worst case, every unescaped character corresponds to 6 escape characters.
     * Examples:
     *   "'" => "&apos;"
     *   """ => "&quot;"
     * 
     * +1 for the NULL byte at the end of the string
     */
    char* escaped = malloc(6 * unescaped_len + 1);

    size_t escaped_pos = 0;
    for (size_t unescaped_pos = 0; unescaped_pos < unescaped_len; unescaped_pos++) {
        switch (unescaped[unescaped_pos]) {
            case '<':
                strncpy(escaped + escaped_pos, "&lt;", 4);
                escaped_pos += 4;
                break;
            case '>':
                strncpy(escaped + escaped_pos, "&gt;", 4);
                escaped_pos += 4;
                break;
            case '&':
                strncpy(escaped + escaped_pos, "&amp;", 5);
                escaped_pos += 5;
                break;
            case '\'':
                strncpy(escaped + escaped_pos, "&apos;", 6);
                escaped_pos += 6;
                break;
            case '\"':
                strncpy(escaped + escaped_pos, "&quot;", 6);
                escaped_pos += 6;
                break;
            default:
                strncpy(escaped + escaped_pos, unescaped + unescaped_pos, 1);
                escaped_pos += 1;
                break;
        }
    }

    escaped[escaped_pos] = '\0';
    return escaped;
}

/* Returns true if str has the prefix pre, otherwise false. */
bool prefix(const char* pre, const char* str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

/* Moves pos index to next non-whitespace or NULL character in str. */
void skip_whitespace(const char* str, size_t* pos) {
    while (str[*pos] != '\0' && (str[*pos] == ' ' || str[*pos] == '\t')) (*pos)++;
}

/* Moves pos index to next whitespace or NULL character in str. */
void skip_non_whitespace(const char* str, size_t* pos) {
    while (str[*pos] != '\0' && !(str[*pos] == ' ' || str[*pos] == '\t')) (*pos)++;
}

/* Escapes unescaped, prepending opening_tag and appending closing tag and a newline.
 * 
 * If unescaped ends in a newline, that newline is ignored. Inserting it causes the returned string to line break inappropriately.
 * 
 * Examples:
 *   escape_text_add_tags("<p>", "5 < 6\n", "</p>", 6) => "<p>5 &lt; 6</p>\n"
 *   escape_text_add_tags("<p>", "5 < 6", "</p>", 5)   => "<p>5 &lt; 6</p>\n"
 *   escape_text_add_tags("", "", "", 0)               => "\n"
 *   escape_text_add_tags("", "", "<br/>", 0)          => "<br/>\n"
 */
char* escape_text_add_tags(const char* opening_tag, const char* unescaped, const char* closing_tag, const size_t unescaped_len) {
    char* escaped = escape_text(unescaped, unescaped_len);

    size_t opening_tag_len = strlen(opening_tag);
    size_t escaped_len = strlen(escaped);
    size_t closing_tag_len = strlen(closing_tag);

    char* escaped_with_tags = malloc(opening_tag_len + escaped_len + closing_tag_len + 2);
    size_t index = 0;

    /* append opening tag */
    strncpy(escaped_with_tags + index, opening_tag, opening_tag_len);
    index += opening_tag_len;

    /* append escaped text */
    strncpy(escaped_with_tags + index, escaped, escaped_len);

    /* don't increment index if newline so newline can be overwritten */
    index += escaped_len - 1;
    if (escaped_with_tags[index] != '\n') index++;

    /* escaped string is no longer needed */
    free(escaped);
    escaped = NULL;

    /* append closing tag */
    strncpy(escaped_with_tags + index, closing_tag, closing_tag_len);
    index += closing_tag_len;

    escaped_with_tags[index++] = '\n';
    escaped_with_tags[index] = '\0';

    return escaped_with_tags;
}

/* Having more specific linetypes than the official Gemini spec turns out to be useful. */
enum linetype{Text, Pre, Pre_toggle, Link, H1, H2, H3, Ul, Quote};

/* Returns the line type.
 * 
 * Gemini line type can be quickly determined based on the first three characters of the line.
 * It's impossible to distinguish between preformatted text and text given only the line, so pre_is_toggled is used as a hint.
 */
enum linetype getlinetype(const char* line, const bool pre_is_toggled) {
    /* the order of the if-else statements is important */
    if      (prefix("```", line)) return Pre_toggle;
    else if (pre_is_toggled)      return Pre;
    else if (prefix("###", line)) return H3;
    else if (prefix("##",  line)) return H2;
    else if (prefix("#",   line)) return H1;
    else if (prefix("=>",  line)) return Link;
    else if (prefix("*",   line)) return Ul;
    else if (prefix(">",   line)) return Quote;
    else                          return Text;
}

/* Converts text line to HTML.
 * 
 * If the text line is empty except for a newline, "<br/>\n" is returned.
 * Otherwise the escaped text line is returned.
 */
char* convert_text_line(const char* line, const size_t len) {
    if (line[0] == '\n') return escape_text_add_tags("", "", "<br/>", 0);
    else return escape_text_add_tags("<p>", line, "</p>", len);
}

/* Converts the preformatted text line to HTML.
 * 
 * Returns the escaped preformatted text line.
 */
char* convert_pre_line(const char* line, const size_t len) {
    return escape_text_add_tags("", line, "", len);
}

/* Converts the preformatting toggle line to HTML.
 * 
 * Returns the empty string. Preformatting toggle lines aren't meant to be displayed.
 * convert_pre_toggle_line() doesn't know if the starting or ending toggle line, so gmi_to_html() handles the pre tags.
 */
char* convert_pre_toggle_line(const char* line, const size_t len) {
    return strdup("");
}

/* Converts the link text line to HTML.
 * 
 * Returns the escaped href and link text surrounded by a tags, appending a newline if there isn't one.
 * The heading prefix "=>" and any immediately following whitespace (spaces and tabs) is omitted.
 * If no link text is provided, the href is used as the link text.
 * 
 * Examples:
 * convert_link_line("=>   https://google.com Goolag ") => "<p><a href=\"https://google.com\"">Goolag </a></p>\n"
 * convert_link_line("=>   https://google.com") => "<p><a href=\"https://google.com\"">https://google.com</a></p>\n"
 */
char* convert_link_line(const char* line, const size_t len) {
    /* skip whitespace before href */
    size_t before_href = 2;
    skip_whitespace(line, &before_href);

    /* find end of href */
    size_t after_href = before_href;
    skip_non_whitespace(line, &after_href);

    /* find beginning of link text */
    size_t before_link_text = after_href;
    skip_whitespace(line, &before_link_text);

    char* escaped_href = escape_text_add_tags("<p><a href=\"", line + before_href, "\">",
                                              after_href - before_href);

    /* escaped_href will be appended to, so remove newline */
    escaped_href[strlen(escaped_href) - 1] = '\0';

    char* escaped_with_tags = NULL;

    /* if no link text exists, use the href as link text */
    if (line[before_link_text] == '\0') {
        escaped_with_tags = escape_text_add_tags(escaped_href, line + before_href, "</a></p>", after_href - before_href);
    /* otherwise use the link text */
    } else {
        escaped_with_tags = escape_text_add_tags(escaped_href, line + before_link_text, "</a></p>", len - before_link_text);
    }

    /* escaped_href is no longer needed */
    free(escaped_href);
    escaped_href = NULL;

    return escaped_with_tags;
}

/* Converts the heading text line to HTML.
 * 
 * Returns the escaped heading text surrounded by h1 tags, appending a newline if there isn't one.
 * The heading prefix "#" and any immediately following whitespace (spaces and tabs) is omitted.
 * 
 * Example:
 * convert_h1_line("#   i'm an h1 heading! ") => "<h1>i&apos;m an h1 heading! </h1>\n"
 */
char* convert_h1_line(const char* line, const size_t len) {
    size_t pos = 1;
    skip_whitespace(line, &pos);
    return escape_text_add_tags("<h1>", line + pos, "</h1>", len - pos);
}

/* Converts the heading text line to HTML.
 * 
 * Returns the escaped heading text surrounded by h2 tags, appending a newline if there isn't one.
 * The heading prefix "##" and any immediately following whitespace (spaces and tabs) is omitted.
 * 
 * Example:
 * convert_h2_line("##   i'm an h2 heading! ") => "<h2>i&apos;m an h2 heading! </h2>\n"
 */
char* convert_h2_line(const char* line, const size_t len) {
    size_t pos = 2;
    skip_whitespace(line, &pos);
    return escape_text_add_tags("<h2>", line + pos, "</h2>", len - pos);
}

/* Converts the heading text line to HTML.
 * 
 * Returns the escaped heading text surrounded by h3 tags, appending a newline if there isn't one.
 * The heading prefix "###" and any immediately following whitespace (spaces and tabs) is omitted.
 * 
 * Example:
 * convert_h3_line("###   i'm an h3 heading! ") => "<h3>i&apos;m an h3 heading! </h3>\n"
 */
char* convert_h3_line(const char* line, const size_t len) {
    size_t pos = 3;
    skip_whitespace(line, &pos);
    return escape_text_add_tags("<h3>", line + pos, "</h3>", len - pos);
}

/* Converts the unordered list line to HTML.
 * 
 * Returns the escaped list text surrounded by list tags, appending a newline if there isn't one.
 * The list line prefix "*" and any immediately following whitespace (spaces and tabs) is omitted.
 * 
 * Example:
 * convert_ul_line("*   i'm a list item! ") => "<li>i&apos;m a list item! </li>\n"
 * 
 * convert_ul_line() doesn't know when it's at the first or last list item, so gmi_to_html() handles the surrounding ul tags.
 */
char* convert_ul_line(const char* line, const size_t len) {
    size_t pos = 1;
    skip_whitespace(line, &pos);
    escape_text_add_tags("<li>", line + pos, "</li>", len - pos);
}

/* Converts the quote line to HTML.
 * 
 * Returns the escaped quote text, appending a newline if there isn't one.
 * The quote line prefix ">" and any immediately following whitespace (spaces and tabs) is omitted.
 * 
 * Example:
 * convert_quote_line(">   i'm a quote item! ") => "i&apos;m a quote item! \n"
 * 
 * convert_quote_line() doesn't know when it's at the first or last quote line, so gmi_to_html() handles the surrounding blockquote tags.
 */
char* convert_quote_line(const char* line, const size_t len) {
    size_t pos = 1;
    skip_whitespace(line, &pos);
    return escape_text_add_tags("", line + pos, "", len - pos);
}

/* Generic function for other convert_line functions. */
char* convert_line(const char* line, const size_t size, const enum linetype type) {
    switch (type) {
        case Text:       return convert_text_line(line, size);
        case Pre:        return convert_pre_line(line, size);
        case Pre_toggle: return convert_pre_toggle_line(line, size);
        case Link:       return convert_link_line(line, size);
        case H1:         return convert_h1_line(line, size);
        case H2:         return convert_h2_line(line, size);
        case H3:         return convert_h3_line(line, size);
        case Ul:         return convert_ul_line(line, size);
        case Quote:      return convert_quote_line(line, size);
    }
}

/* Reads in Gemini from standard in, outputs HTML to standard out.
 * 
 * Returns 0 if file conversion is successful, -1 if file reading/writing fails.
 * Reads standard in line by line, converting each line to HTML and adding tags as necessary.
 */
int gmi_to_html(FILE* fp_gmi, FILE* fp_html) {
    if (fp_gmi == NULL || fp_html == NULL) return -1;

    char* line = NULL;
    char* converted = NULL;

    size_t len = 0;
    ssize_t read;

    /* cur and prev help determine when to add HTML tags */
    enum linetype cur = Text;
    enum linetype prev = Text;

    bool pre_is_toggled = false;

    while ((read = getline(&line, &len, fp_gmi)) != -1) {
        cur = getlinetype(line, pre_is_toggled);

        if (cur != Pre_toggle && pre_is_toggled) {
            cur = Pre;
        } else if (cur == Pre_toggle) {
            pre_is_toggled = !pre_is_toggled;
            if (pre_is_toggled) fputs("<pre>\n", fp_html);
            else fputs("</pre>\n", fp_html);
        } else {
            /* closing tags must be written before opening tags */
            if(cur != Ul && prev == Ul) fputs("</ul>\n", fp_html);
            else if (cur != Quote && prev == Quote) fputs("</blockquote>\n", fp_html);

            if(cur == Ul && prev != Ul) fputs("<ul>\n", fp_html);
            else if (cur == Quote && prev != Quote) fputs("<blockquote>\n", fp_html);
        }

        converted = convert_line(line, strlen(line), cur);
        fputs(converted, fp_html);

        free(converted);
        converted = NULL;

        prev = cur;
    }

    free(line);
    line = NULL;

    /* if a Pre, Ul, or Quote line is the last line, the getline loop won't automatically close its tag */
    switch (cur) {
        case Pre: fputs("</pre>\n", fp_html);          break;
        case Ul:  fputs("</ul>\n", fp_html);           break;
        case Quote: fputs("</blockquote>\n", fp_html); break;
    }

    return 0;
}
