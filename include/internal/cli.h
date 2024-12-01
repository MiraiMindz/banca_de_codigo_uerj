/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#ifndef INTERNAL_CLI_H
#define INTERNAL_CLI_H

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

void tc_set_color(cli_colors_t foreground, cli_colors_t background);
void tc_reset_color(void);

#endif /* INTERNAL_CLI_H */
