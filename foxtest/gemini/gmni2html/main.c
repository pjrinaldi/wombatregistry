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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "gemini2html.h"

#define VERSION "1.3.4"

/* displays the help menu */
void display_help_text(char* prog) {
    char* help_text =
        "Convert Gemini files to HTML.\n"
        "\n"
        "OPTIONS:\n"
        "  -h, --help      display this help text then exit\n"
        "  -v, --version   display program version number then exit\n"
        "\n";

    printf("Usage: %s [OPTION]... < input.gmi > output.html\n%s", prog, help_text);
}

/* displays version information */
void display_version_text(char* prog) {
    printf("%s v%s\n", prog, VERSION);
}

/* CLI for gemini2html */
int main(int argc, char* argv[]) {
    int c;

    while (1) {
        static struct option long_options[] = {
            {"help",    no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "hv", long_options, &option_index);

        if (c == -1) break;

        switch(c) {
            case 'h': display_help_text(argv[0]); exit (EXIT_SUCCESS);
            case 'v': display_version_text(argv[0]); exit (EXIT_SUCCESS);
            default: return -1;
        }
    }

    if (gmi_to_html(stdin, stdout) == -1) return -1;
    else return 0;
}
