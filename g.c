#include <stdio.h>
#include <ctype.h>

typedef enum cores_terminal_e {
    PADRAO,
    PRETO_CLARO,
    VERMELHO_CLARO,
    VERDE_CLARO,
    AMARELO_CLARO,
    AZUL_CLARO,
    MAGENTA_CLARO,
    CIANO_CLARO,
    BRANCO_CLARO,
    PRETO_ESCURO,
    VERMELHO_ESCURO,
    VERDE_ESCURO,
    AMARELO_ESCURO,
    AZUL_ESCURO,
    MAGENTA_ESCURO,
    CIANO_ESCURO,
    BRANCO_ESCURO
} cores_terminal_t;

void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    if (cor_texto >= PRETO_CLARO && cor_texto <= BRANCO_ESCURO) {
        int ansi_fg = (cor_texto >= PRETO_ESCURO)
                          ? (30 + (cor_texto - PRETO_ESCURO))
                          : (90 + (cor_texto - PRETO_CLARO));
        printf("\033[%dm", ansi_fg); /* cor_texto */
    }
    if (cor_fundo >= PRETO_CLARO && cor_fundo <= BRANCO_ESCURO) {
        int ansi_bg = (cor_fundo >= PRETO_ESCURO)
                          ? (40 + (cor_fundo - PRETO_ESCURO))
                          : (100 + (cor_fundo - PRETO_CLARO));
        printf("\033[%dm", ansi_bg); /* cor_fundo */
    }
}

void cli_redefinir_cores(void) { printf("\033[0m"); }

void cli_processar_string(char *str) {
    char *ptr = str;
    while (*ptr != '\0') {
        if (*ptr == '\033') { // Start of an escape sequence
            char *esc_start = ptr;
            ptr++; // Skip '\033'
            if (*ptr == '[') {
                ptr++; // Skip '['
                int is_reset = 0;
                int fg_color = -1, bg_color = -1;
                
                while (isdigit(*ptr) || *ptr == ';') {
                    int value = 0;
                    while (isdigit(*ptr)) { // Parse a number
                        value = value * 10 + (*ptr - '0');
                        ptr++;
                    }

                    if (value == 0) { // Reset code
                        is_reset = 1;
                    } else if (value >= 30 && value <= 37) { // Foreground bright
                        fg_color = value - 30 + PRETO_ESCURO;
                    } else if (value >= 90 && value <= 97) { // Foreground light
                        fg_color = value - 90 + PRETO_CLARO;
                    } else if (value >= 40 && value <= 47) { // Background bright
                        bg_color = value - 40 + PRETO_ESCURO;
                    } else if (value >= 100 && value <= 107) { // Background light
                        bg_color = value - 100 + PRETO_CLARO;
                    }

                    if (*ptr == ';') ptr++; // Skip separator
                }

                if (*ptr == 'm') {
                    ptr++; // Skip 'm' at the end of the sequence
                    if (is_reset) {
                        cli_redefinir_cores();
                    } else {
                        cli_definir_cores(fg_color, bg_color);
                    }
                }
            }
        } else {
            putchar(*ptr); // Print regular characters
            ptr++;
        }
    }
}

int main() {
    char *test_str = "\033[0m\033[30;46m#include \033[36;40m<stdio.h>\033[0m\n";
    cli_processar_string(test_str);
    return 0;
}
