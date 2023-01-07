#ifndef GEMINI2HTML_H
#define GEMINI2HTML_H
/*
 * Reads in Gemini from standard in, outputs HTML to standard out.
 * Returns 0 if file conversion is successful, -1 if file reading/writing fails.
 */
int gmi_to_html(FILE* fp_gmi, FILE* fp_html);
#endif