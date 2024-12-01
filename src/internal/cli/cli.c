/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include "internal/cli.h"
#include <stdio.h>

#ifdef _WIN32
/* Windows-specific color values */
static const int win_colors[] = {
    0,                      // RESET (not used in Windows API)
    0,                      // LIGHT_BLACK (not directly supported, map to BLACK)
    FOREGROUND_RED | FOREGROUND_INTENSITY,         // LIGHT_RED
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,       // LIGHT_GREEN
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // LIGHT_YELLOW
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,        // LIGHT_BLUE
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,  // LIGHT_MAGENTA
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // LIGHT_CYAN
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // LIGHT_WHITE
    0,                      // DARK_BLACK (map to BLACK)
    FOREGROUND_RED,         // DARK_RED
    FOREGROUND_GREEN,       // DARK_GREEN
    FOREGROUND_RED | FOREGROUND_GREEN,            // DARK_YELLOW
    FOREGROUND_BLUE,        // DARK_BLUE
    FOREGROUND_RED | FOREGROUND_BLUE,             // DARK_MAGENTA
    FOREGROUND_GREEN | FOREGROUND_BLUE,           // DARK_CYAN
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE // DARK_WHITE
};

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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

#else
/* Unix-like systems */
void tc_set_color(cli_colors_t foreground, cli_colors_t background) {
    if (foreground >= LIGHT_BLACK && foreground <= DARK_WHITE) {
        int ansi_fg = (foreground >= DARK_BLACK) ? (30 + (foreground - DARK_BLACK)) : (90 + (foreground - LIGHT_BLACK));
        printf("\033[%dm", ansi_fg); // Foreground
    }
    if (background >= LIGHT_BLACK && background <= DARK_WHITE) {
        int ansi_bg = (background >= DARK_BLACK) ? (40 + (background - DARK_BLACK)) : (100 + (background - LIGHT_BLACK));
        printf("\033[%dm", ansi_bg); // Background
    }
}

void tc_reset_color(void) {
    printf("\033[0m");
}

#endif
