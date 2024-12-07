/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include "config/config.h"
#include <internal/cli.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <malloc.h>
#include <windows.h>
#define limpar_tela() system("cls")
#else
#include <alloca.h>
#define limpar_tela() system("clear")
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifdef _WIN32

/* Windows-specific color values */
static const int win_colors[] = {
    0, /* RESET (Não usado pela API do Windows) */
    0, /* PRETO_CLARO não existe na API do Windows, usamos PRETO_ESCURO */
    FOREGROUND_RED | FOREGROUND_INTENSITY,                    /* LIGHT_RED */
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,                  /* LIGHT_GREEN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, /* LIGHT_YELLOW */
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,                   /* LIGHT_BLUE */
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, /* LIGHT_MAGENTA */
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, /* LIGHT_CYAN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY,                           /* BRANCO_CLARO */
    0,                                                  /* PRETO_ESCURO */
    FOREGROUND_RED,                                     /* DARK_RED */
    FOREGROUND_GREEN,                                   /* DARK_GREEN */
    FOREGROUND_RED | FOREGROUND_GREEN,                  /* DARK_YELLOW */
    FOREGROUND_BLUE,                                    /* DARK_BLUE */
    FOREGROUND_RED | FOREGROUND_BLUE,                   /* DARK_MAGENTA */
    FOREGROUND_GREEN | FOREGROUND_BLUE,                 /* DARK_CYAN */
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE /* BRANCO_ESCURO */
};

#if _USAR_COR_NO_TERMINAL == 1
void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_attr = 0;

    /* Map cor_texto */
    if (cor_texto >= PRETO_CLARO && cor_texto <= BRANCO_ESCURO) {
        color_attr |= win_colors[cor_texto];
    }

    /* Map cor_fundo (shifted 4 bits for Windows attributes) */
    if (cor_fundo >= PRETO_CLARO && cor_fundo <= BRANCO_ESCURO) {
        color_attr |= (win_colors[cor_fundo] << 4);
    }

    SetConsoleTextAttribute(hConsole, color_attr);
}

void cli_redefinir_cores() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#else
void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    (void)cor_texto;
    (void)cor_fundo;
    /* No-op when colors are disabled */
}

void cli_redefinir_cores() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#endif

#else

#if _USAR_COR_NO_TERMINAL == 1
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
#else
void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    (void)cor_texto;
    (void)cor_fundo;
    /* No-op when colors are disabled */
}

void cli_redefinir_cores(void) { printf("\033[0m"); }
#endif
#endif

void exibir_linha_topo_interface(void) {
    printf("/--------------------------------------------------------------"
           "----------------\\\n");
}

void exibir_linha_inferior_interface(void) {
    printf("\\-------------------------------------------------------------"
           "-----------------/\n\n");
}

void exibir_divisor_interface(void) {
    printf("|--------------------------------------------------------------"
           "----------------|\n");
}

void exibir_espacador_interface(void) { printf("|%78s|\n", ""); }

static unsigned long contar_caracteres_utf8(const char *texto) {
    unsigned long quantidade;
    quantidade = 0;

    while (*texto) {
        /* Se o primeiro byte estiver no intervalo 0x00-0x7F, é um caractere de
         * 1 byte (ASCII). */
        if ((*texto & 0x80) == 0) {
            texto++;
        }
        /* Se o primeiro byte estiver no intervalo 0xC0-0xDF, é um caractere de
           2 bytes. */
        else if ((*texto & 0xE0) == 0xC0) {
            texto += 2;
        }
        /* Se o primeiro byte estiver no intervalo 0xE0-0xEF, é um caractere de
           3 bytes. */
        else if ((*texto & 0xF0) == 0xE0) {
            texto += 3;
        }
        /* Se o primeiro byte estiver no intervalo 0xF0-0xFF, é um caractere de
           4 bytes. */
        else if ((*texto & 0xF8) == 0xF0) {
            texto += 4;
        }
        quantidade++;
    }

    return quantidade;
}

char *printf_para_string(const char *formato, va_list lista_de_argumentos) {
    int tamanho;
    char *resultado;
    resultado = calloc(255, sizeof(char));
    tamanho = vsprintf(resultado, formato, lista_de_argumentos);

    if (tamanho < 0) {
        free(resultado);
        return NULL;
    }

    resultado = realloc(resultado, tamanho + 1);
    if (resultado == NULL) {
        free(resultado);
        return NULL;
    }

    vsprintf(resultado, formato, lista_de_argumentos);
    return resultado;
}

void exibir_linha_textual_interface(char *formato,
                                    alinhamento_textual_t alinhamento,
                                    cores_terminal_t cor_texto,
                                    cores_terminal_t cor_fundo, ...) {
    va_list lista_de_argumentos;
    unsigned long largura_do_texto;
    unsigned long espacamento_total;
    unsigned long i;
    char *texto;
    va_start(lista_de_argumentos, cor_fundo);

    espacamento_total = 0;
    texto = printf_para_string(formato, lista_de_argumentos);
    largura_do_texto = contar_caracteres_utf8(texto);

    printf("| ");
    if (largura_do_texto <= 74) {
        espacamento_total = (74 - largura_do_texto) + 1;
        switch (alinhamento) {
        case ESQUERDA:
            cli_definir_cores(cor_texto, cor_fundo);
            printf("%s", texto);
            for (i = 0; i <= espacamento_total; i++) {
                putchar(' ');
            }
            cli_redefinir_cores();
            break;
        case CENTRO:
            cli_definir_cores(cor_texto, cor_fundo);
            if ((largura_do_texto % 2) == 0) {
                for (i = 0; i <= (espacamento_total / 2); i++) {
                    putchar(' ');
                }
                printf("%s", texto);
                for (i = 0; i <= (espacamento_total / 2); i++) {
                    putchar(' ');
                }
            } else {
                for (i = 0; i <= ((espacamento_total / 2) - 1); i++) {
                    putchar(' ');
                }
                printf("%s", texto);
                for (i = 0; i <= (espacamento_total / 2); i++) {
                    putchar(' ');
                }
            }
            cli_redefinir_cores();
            break;
        case DIREITA:
            cli_definir_cores(cor_texto, cor_fundo);
            for (i = 0; i <= espacamento_total; i++) {
                putchar(' ');
            }
            printf("%s", texto);
            cli_redefinir_cores();
            break;
        }
    } else if ((largura_do_texto > 74) && largura_do_texto < 76) {
        cli_definir_cores(cor_texto, cor_fundo);
        printf("%s ", texto);
        cli_redefinir_cores();
    } else {
        cli_definir_cores(cor_texto, cor_fundo);
        printf("%s", texto);
        cli_redefinir_cores();
    }
    printf(" |\n");

    free(texto);
    cli_redefinir_cores();
    va_end(lista_de_argumentos);
}

void exibir_opcao_interface(unsigned long numero, char *opcao) {
    printf("[");
    cli_definir_cores(BRANCO_CLARO, PADRAO);
    printf("%lu", numero);
    cli_redefinir_cores();
    printf("] - %s\n", opcao);
}

void exibir_opcao_configurar_display(unsigned long numero, char *opcao,
                                     char *descricao,
                                     unsigned char condicional) {
    printf("\t(%d) - %s [", numero, opcao);
    if (condicional != 0) {
        cli_definir_cores(VERDE_CLARO, PADRAO);
        printf("SELECIONADO");
        cli_redefinir_cores();
    } else {
        cli_definir_cores(AZUL_CLARO, PADRAO);
        printf("DISPONIVEL");
        cli_redefinir_cores();
    }
    printf("]\n");
    printf("\t\t%s\n", descricao);
}

char **dividir_texto(const char *texto, int largura, int *contagem_de_linhas) {
    unsigned long tamanho_do_buffer;
    char **resultado;
    const char *inicio;
    char *resultado_ptr;
    const char *espaco_ptr;
    int tamanho_atual;
    unsigned long tamanho_usado;
    int contagem_de_espacos;
    int i;
    const char *atual;
    const char *fim;
    unsigned long tamanho_da_linha;
    char **novo_resultado;

    tamanho_do_buffer = 256;
    resultado = malloc(tamanho_do_buffer * sizeof(char *));
    if (!resultado) {
        return NULL;
    }

    inicio = texto;
    resultado_ptr = NULL;
    espaco_ptr = NULL;
    tamanho_atual = 0;
    tamanho_usado = 0;
    contagem_de_espacos = 0;
    *contagem_de_linhas = 0;

    while (*inicio != '\0') {
        atual = inicio;
        tamanho_atual = 0;
        espaco_ptr = NULL;
        contagem_de_espacos = 0;

        while (*atual != '\0' && tamanho_atual < largura) {
            if (*atual == ' ') {
                espaco_ptr = atual;
                contagem_de_espacos++;
            }
            atual++;
            tamanho_atual++;
        }

        if (contagem_de_espacos > 1) {
            if (espaco_ptr != NULL && tamanho_atual <= largura) {
                fim = espaco_ptr;
                contagem_de_espacos = 0;
            } else {
                while (*atual != ' ' && *atual != '\0') {
                    atual++;
                }
                fim = atual;
            }
        } else {
            fim = atual;
        }

        tamanho_da_linha = fim - inicio + 1;
        if (tamanho_usado + tamanho_da_linha >= tamanho_do_buffer) {
            tamanho_do_buffer = (tamanho_usado + tamanho_da_linha) * 2;
            novo_resultado =
                realloc(resultado, tamanho_do_buffer * sizeof(char *));
            if (!novo_resultado) {
                free(resultado);
                return NULL;
            }
            resultado = novo_resultado;
        }

        resultado[*contagem_de_linhas] = malloc(tamanho_da_linha + 1);
        if (!resultado[*contagem_de_linhas]) {
            for (i = 0; i < *contagem_de_linhas; i++) {
                free(resultado[i]);
            }
            free(resultado);
            return NULL;
        }

        resultado_ptr = resultado[*contagem_de_linhas];
        while (inicio < fim) {
            *resultado_ptr++ = *inicio++;
        }

        *resultado_ptr = '\0';
        (*contagem_de_linhas)++;

        while (*inicio == ' ') {
            inicio++;
        }
    }

    return resultado;
}



void paginador(char *texto) {
    int altura_da_tela;
    int largura_da_tela;
    char opcao;
    char **texto_divido;
    int i;
    int contagem_de_linhas;
    int total_de_linhas;
    int linha_atual;
    int sair;

    altura_da_tela = 24;
    largura_da_tela = 80;
    linha_atual = 0;
    sair = 1;

    texto_divido = dividir_texto(texto, largura_da_tela, &contagem_de_linhas);

    while (sair == 1) {
        limpar_tela();
        total_de_linhas = linha_atual + altura_da_tela;
        if (total_de_linhas > contagem_de_linhas) {
            total_de_linhas = contagem_de_linhas;
        }

        for (i = linha_atual; i < total_de_linhas; i++) {
            printf("%s\n", texto_divido[i]);
        }

        printf("\n--More-- (%d/%d lines)\n", total_de_linhas,
               contagem_de_linhas);
        opcao = getchar();

        if (opcao == 'q') {
            sair = 0;
            break;
        } else if (opcao == 'u') {
            if (linha_atual > 0) {
                linha_atual--;
            }
        } else if (opcao == 'd') {
            if ((linha_atual < contagem_de_linhas - 1) &&
                (total_de_linhas != contagem_de_linhas)) {
                linha_atual++;
            }
        } else if (opcao == ' ') {
            linha_atual += altura_da_tela;
            if (linha_atual >= contagem_de_linhas) {
                linha_atual = contagem_de_linhas - 1;
            }
        }
    }

    for (i = 0; i < contagem_de_linhas; i++) {
        free(texto_divido[i]);
    }
    free(texto_divido);
}
