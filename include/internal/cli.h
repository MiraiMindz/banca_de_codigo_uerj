#ifndef INTERNAL_CLI_H
#define INTERNAL_CLI_H

typedef enum cli_colors_e {
    DEFAULT,
    LIGHT_BLACK,
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW,
    LIGHT_BLUE,
    LIGHT_MAGENTA,
    LIGHT_CYAN,
    LIGHT_WHITE,
    DARK_BLACK,
    DARK_RED,
    DARK_GREEN,
    DARK_YELLOW,
    DARK_BLUE,
    DARK_MAGENTA,
    DARK_CYAN,
    DARK_WHITE
} cli_colors_t;

char* color_text(char* string, cli_colors_t color);                                                                                             
void display_text(const char* format, ...);

#ifdef _WIN32
#include <windows.h>

/* Map ANSI escape codes to Windows Console attributes */
WORD get_windows_color(const char* ansi_code);
#endif

#endif /* INTERNAL_CLI_H */
