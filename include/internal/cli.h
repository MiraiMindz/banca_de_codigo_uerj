/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#ifndef INTERNAL_CLI_H
#define INTERNAL_CLI_H

#include <stdarg.h>
#include <config/config.h>

typedef enum cli_colors_e {
    RESET,
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

typedef enum format_alignment_e {
    LEFT,
    CENTER,
    RIGHT
} format_alignment_t;

void tc_set_color(cli_colors_t foreground, cli_colors_t background);
void tc_reset_color(void);
void print_interface_header(void);
void print_interface_footer(void);
void print_interface_divider(void);
void print_interface_padder(void);
void print_interface_line(char *format, format_alignment_t alignment, cli_colors_t fg_color, cli_colors_t bg_color, ...);
void print_interface_option(unsigned long number, char* option_name);

#endif /* INTERNAL_CLI_H */
