/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include <internal/cli.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

#ifdef _WIN32
#include <windows.h>

/* Windows-specific color values */
static const int win_colors[] = {
    0, /* RESET (not used in Windows API) */
    0, /* LIGHT_BLACK (not directly supported, map to BLACK) */
    FOREGROUND_RED | FOREGROUND_INTENSITY,                     /* LIGHT_RED */
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,                   /* LIGHT_GREEN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,  /* LIGHT_YELLOW */
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,                    /* LIGHT_BLUE */
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,   /* LIGHT_MAGENTA */
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, /* LIGHT_CYAN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY,           /* LIGHT_WHITE */
    0,                                  /* DARK_BLACK (map to BLACK) */
    FOREGROUND_RED,                     /* DARK_RED */
    FOREGROUND_GREEN,                   /* DARK_GREEN */
    FOREGROUND_RED | FOREGROUND_GREEN,  /* DARK_YELLOW */
    FOREGROUND_BLUE,                    /* DARK_BLUE */
    FOREGROUND_RED | FOREGROUND_BLUE,   /* DARK_MAGENTA */
    FOREGROUND_GREEN | FOREGROUND_BLUE, /* DARK_CYAN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE /* DARK_WHITE */
};

#if _USE_COLOR == 1
void tc_set_color(cli_colors_t foreground, cli_colors_t background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_attr = 0;

    /* Map foreground */
    if (foreground >= LIGHT_BLACK && foreground <= DARK_WHITE) {
        color_attr |= win_colors[foreground];
    }

    /* Map background (shifted 4 bits for Windows attributes) */
    if (background >= LIGHT_BLACK && background <= DARK_WHITE) {
        color_attr |= (win_colors[background] << 4);
    }

    SetConsoleTextAttribute(hConsole, color_attr);
}

void tc_reset_color() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#else
void tc_set_color(cli_colors_t foreground, cli_colors_t background) {
    (void)foreground;
    (void)background;
    /* No-op when colors are disabled */
}

void tc_reset_color() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#endif

#else
/* Unix-like systems */
#include <stdio.h>

#if _USE_COLOR == 1
void tc_set_color(cli_colors_t foreground, cli_colors_t background) {
    if (foreground >= LIGHT_BLACK && foreground <= DARK_WHITE) {
        int ansi_fg = (foreground >= DARK_BLACK)
                          ? (30 + (foreground - DARK_BLACK))
                          : (90 + (foreground - LIGHT_BLACK));
        printf("\033[%dm", ansi_fg); /* Foreground */
    }
    if (background >= LIGHT_BLACK && background <= DARK_WHITE) {
        int ansi_bg = (background >= DARK_BLACK)
                          ? (40 + (background - DARK_BLACK))
                          : (100 + (background - LIGHT_BLACK));
        printf("\033[%dm", ansi_bg); /* Background */
    }
}

void tc_reset_color(void) { printf("\033[0m"); }
#else
void tc_set_color(cli_colors_t foreground, cli_colors_t background) {
    (void)foreground;
    (void)background;
    /* No-op when colors are disabled */
}

void tc_reset_color(void) { printf("\033[0m"); }
#endif
#endif


void print_interface_header(void) {
    printf("/--------------------------------------------------------------"
           "----------------\\\n");
}

void print_interface_footer(void) {
    printf("\\-------------------------------------------------------------"
           "-----------------/\n\n");
}

void print_interface_divider(void) {
    printf("|--------------------------------------------------------------"
           "----------------|\n");
}

void print_interface_padder(void) { printf("|%78s|\n", ""); }

unsigned long count_utf8_characters(const char *str) {
    unsigned long count;
    count = 0;

    while (*str) {
        /* If the first byte has a value in the range 0x00-0x7F, it's a 1-byte
         * character (ASCII). */
        if ((*str & 0x80) == 0) {
            str++;
        }
        /* If the first byte has a value in the range 0xC0-0xDF, it's a 2-byte
           character. */
        else if ((*str & 0xE0) == 0xC0) {
            str += 2;
        }
        /* If the first byte has a value in the range 0xE0-0xEF, it's a 3-byte
           character. */
        else if ((*str & 0xF0) == 0xE0) {
            str += 3;
        }
        /* If the first byte has a value in the range 0xF0-0xFF, it's a 4-byte
           character. */
        else if ((*str & 0xF8) == 0xF0) {
            str += 4;
        }
        count++;
    }

    return count;
}

char *strprintf(const char *format, va_list va) {
    int length;
    char *result;
    result = calloc(255, sizeof(char));
    length = vsprintf(result, format, va);

    if (length < 0) {
        free(result);
        return NULL;
    }

    result = realloc(result, length + 1);
    if (result == NULL) {
        free(result);
        return NULL;
    }

    vsprintf(result, format, va);
    return result;
}

void print_interface_line(char *format, format_alignment_t alignment,
                          cli_colors_t fg_color, cli_colors_t bg_color, ...) {
    va_list va;
    unsigned long string_width;
    unsigned long padding_total;
    unsigned long i;
    char *string;
    va_start(va, bg_color);

    padding_total = 0;
    string = strprintf(format, va);
    string_width = count_utf8_characters(string);

    printf("| ");
    if (string_width <= 74) {
        padding_total = (74 - string_width) + 1;
        switch (alignment) {
        case LEFT:
            tc_set_color(fg_color, bg_color);
            printf("%s", string);
            for (i = 0; i <= padding_total; i++) {
                putchar(' ');
            }
            tc_reset_color();
            break;
        case CENTER:
            tc_set_color(fg_color, bg_color);
            if ((string_width % 2) == 0) {
                for (i = 0; i <= (padding_total / 2); i++) {
                    putchar(' ');
                }
                printf("%s", string);
                for (i = 0; i <= (padding_total / 2); i++) {
                    putchar(' ');
                }
            } else {
                for (i = 0; i <= ((padding_total / 2) - 1); i++) {
                    putchar(' ');
                }
                printf("%s", string);
                for (i = 0; i <= (padding_total / 2); i++) {
                    putchar(' ');
                }
            }
            tc_reset_color();
            break;
        case RIGHT:
            tc_set_color(fg_color, bg_color);
            for (i = 0; i <= padding_total; i++) {
                putchar(' ');
            }
            printf("%s", string);
            tc_reset_color();
            break;
        }
    } else if ((string_width > 74) && string_width < 76) {
        tc_set_color(fg_color, bg_color);
        printf("%s ", string);
        tc_reset_color();
    } else {
        tc_set_color(fg_color, bg_color);
        printf("%s", string);
        tc_reset_color();
    }
    printf(" |\n");


    free(string);
    tc_reset_color();
    va_end(va);
}

void print_interface_option(unsigned long number, char* option_name) {
    printf("[");
    tc_set_color(LIGHT_WHITE, DARK_BLACK);
    printf("%lu", number);
    tc_reset_color();
    printf("] - %s\n", option_name);
}
