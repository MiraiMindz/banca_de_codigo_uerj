/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include "config/config.h"
#include <ctype.h>
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
/**
 * @name cli_definir_cores
 * @return void
 * @param cor_texto:cores_terminal_t
 * @param cor_fundo:cores_terminal_t
 *
 * Função para definir as cores do texto e do fundo do terminal utilizando
 *códigos ANSI. A função recebe as cores para o texto e o fundo, e utiliza os
 *códigos de escape ANSI para alterar as cores de exibição no terminal.
 **/
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

/**
 * @name cli_redefinir_cores
 * @return void
 *
 * Função para redefinir as cores do terminal para o padrão
 **/
void cli_redefinir_cores() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#else
/**
 * @name cli_definir_cores
 * @return void
 * @param cor_texto:cores_terminal_t
 * @param cor_fundo:cores_terminal_t
 *
 * Função para definir as cores do texto e do fundo do terminal utilizando
 *códigos ANSI. A função recebe as cores para o texto e o fundo, e utiliza os
 *códigos de escape ANSI para alterar as cores de exibição no terminal.
 **/
void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    (void)cor_texto;
    (void)cor_fundo;
    /* No-op when colors are disabled */
}

/**
 * @name cli_redefinir_cores
 * @return void
 *
 * Função para redefinir as cores do terminal para o padrão
 **/
void cli_redefinir_cores() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_BLUE);
}
#endif

#else

#if _USAR_COR_NO_TERMINAL == 1
/**
 * @name cli_definir_cores
 * @return void
 * @param cor_texto:cores_terminal_t
 * @param cor_fundo:cores_terminal_t
 *
 * Função para definir as cores do texto e do fundo do terminal utilizando
 *códigos ANSI. A função recebe as cores para o texto e o fundo, e utiliza os
 *códigos de escape ANSI para alterar as cores de exibição no terminal.
 **/
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

/**
 * @name cli_redefinir_cores
 * @return void
 *
 * Função para redefinir as cores do terminal para o padrão
 **/
void cli_redefinir_cores(void) { printf("\033[0m"); }
#else

/**
 * @name cli_definir_cores
 * @return void
 * @param cor_texto:cores_terminal_t
 * @param cor_fundo:cores_terminal_t
 *
 * Função para definir as cores do texto e do fundo do terminal utilizando
 *códigos ANSI. A função recebe as cores para o texto e o fundo, e utiliza os
 *códigos de escape ANSI para alterar as cores de exibição no terminal.
 **/
void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo) {
    (void)cor_texto;
    (void)cor_fundo;
    /* No-op when colors are disabled */
}

/**
 * @name cli_redefinir_cores
 * @return void
 *
 * Função para redefinir as cores do terminal para o padrão
 **/
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

/**
 * @name contar_caracteres_utf8
 * @return unsigned long
 * @param texto:const char*
 *
 * Função que conta a quantidade de caracteres em uma string codificada em
 *UTF-8. A função verifica o número de bytes usados para cada caractere,
 *considerando os diferentes formatos de codificação de caracteres UTF-8 (1 a 4
 *bytes).
 **/
unsigned long contar_caracteres_utf8(const char *texto) {
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

/**
 * @name printf_para_string
 * @return char* (um ponteiro para a string formatada, ou NULL em caso de erro)
 * @param formato: const char* (a string de formato que define como os
 *argumentos devem ser formatados)
 * @param ...: (argumentos variáveis que serão formatados de acordo com o
 *formato fornecido)
 *
 * A função utiliza um formato fornecido e argumentos variáveis para criar uma
 *string formatada e retornar o resultado em um buffer alocado dinamicamente.
 **/
char *printf_para_string(const char *formato, ...) {
    /* Declarações */
    va_list args;
    int tamanho;
    char *buffer;
    char buffer_temporal[1024]; /* Buffer temporário para estimar o tamanho da
                                   string */

    /* Inicializa a va_list para os argumentos variáveis */
    va_start(args, formato);

    /* Primeira passagem: Formatar a string no buffer temporário para estimar o
     * tamanho */
    tamanho = vsprintf(buffer_temporal, formato, args);

    /* Se o tamanho for negativo, ocorreu um erro na formatação */
    if (tamanho < 0) {
        va_end(args);
        return NULL; /* Retorna NULL em caso de erro */
    }

    /* Aloca memória para o buffer final (tamanho + 1 para o terminador nulo) */
    buffer = (char *)malloc(tamanho + 1);
    if (buffer == NULL) {
        /* Em caso de falha na alocação de memória, libera os recursos e retorna
        NULL */
        free(buffer);
        va_end(args);
        return NULL;
    }

    /* Reinicializa a va_list antes da segunda passagem */
    va_start(args, formato);

    /* Segunda passagem: Formatar a string no buffer alocado dinamicamente */
    vsprintf(buffer, formato, args);

    /* Limpeza da va_list */
    va_end(args);

    return buffer; /* Retorna o ponteiro para a string formatada */
}

/**
 * @name printf_para_string_va
 * @return char* (um ponteiro para a string formatada, ou NULL em caso de erro)
 * @param formato: const char* (a string de formato que define como os
 *argumentos devem ser formatados)
 * @param args:va_list (argumentos variáveis que serão formatados de acordo com
 *o formato fornecido)
 *
 * A função utiliza um formato fornecido e argumentos variáveis para criar uma
 *string formatada e retornar o resultado em um buffer alocado dinamicamente.
 **/
char *printf_para_string_va(const char *formato, va_list args) {
    /* Declarações */
    int tamanho;
    char *buffer;
    char buffer_temporal[1024]; /* Buffer temporário para estimar o tamanho da
                                   string */

    /* Primeira passagem: Formatar a string no buffer temporário para estimar o
     * tamanho */
    tamanho = vsprintf(buffer_temporal, formato, args);

    /* Se o tamanho for negativo, ocorreu um erro na formatação */
    if (tamanho < 0) {
        va_end(args);
        return NULL; /* Retorna NULL em caso de erro */
    }

    /* Aloca memória para o buffer final (tamanho + 1 para o terminador nulo) */
    buffer = (char *)malloc(tamanho + 1);
    if (buffer == NULL) {
        /* Em caso de falha na alocação de memória, libera os recursos e retorna
        NULL */
        free(buffer);
        va_end(args);
        return NULL;
    }

    /* Segunda passagem: Formatar a string no buffer alocado dinamicamente */
    vsprintf(buffer, formato, args);

    return buffer; /* Retorna o ponteiro para a string formatada */
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
    texto = printf_para_string_va(formato, lista_de_argumentos);
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

typedef enum tipo_token_markdown_e {
    TEXTO,
    CABECALHO,
    ITALICO,
    NEGRITO,
    CODIGO,
    BLOCO_CITACAO,
    BLOCO_CODIGO,
    LISTA
} tipo_token_markdown_t;

typedef struct token_markdown_s {
    tipo_token_markdown_t tipo;
    char conteudo[1024];
    int nivel_lista;
    char language[1024];
} token_markdown_t;

/* Lista de palavras-chave do C89 */
const char *palavras_chave[] = {
    "auto",     "break",  "case",    "char",   "const",    "continue",
    "default",  "do",     "double",  "else",   "enum",     "extern",
    "float",    "for",    "goto",    "if",     "int",      "long",
    "register", "return", "short",   "signed", "sizeof",   "static",
    "struct",   "switch", "typedef", "union",  "unsigned", "void",
    "volatile", "while",  NULL};

const char *operadores[] = {
    "+",  "-",  "*",  "/",  "%", "=", "==", "!=", "<", ">",
    "<=", ">=", "&&", "||", "!", "&", "|",  "^",  "~", "<<",
    ">>", "++", "--", "->", ".", "?", ":",  NULL};

/**
 * @name obter_codigo_ansi
 * @return char*
 * @param cor:cores_terminal_t
 * @param tipo:int
 *
 * Função que retorna o código ANSI correspondente a uma cor de primeiro plano
 *(foreground) ou fundo (background), dependendo do tipo fornecido. O tipo 0
 *corresponde ao primeiro plano e o tipo 1 ao fundo.
 **/
char *obter_codigo_ansi(cores_terminal_t cor, int tipo) {
    if (tipo == 0) { // Foreground
        switch (cor) {
        case PADRAO:
            return "\033[0m";
        case PRETO_CLARO:
            return "\033[90m";
        case VERMELHO_CLARO:
            return "\033[91m";
        case VERDE_CLARO:
            return "\033[92m";
        case AMARELO_CLARO:
            return "\033[93m";
        case AZUL_CLARO:
            return "\033[94m";
        case MAGENTA_CLARO:
            return "\033[95m";
        case CIANO_CLARO:
            return "\033[96m";
        case BRANCO_CLARO:
            return "\033[97m";
        case PRETO_ESCURO:
            return "\033[30m";
        case VERMELHO_ESCURO:
            return "\033[31m";
        case VERDE_ESCURO:
            return "\033[32m";
        case AMARELO_ESCURO:
            return "\033[33m";
        case AZUL_ESCURO:
            return "\033[34m";
        case MAGENTA_ESCURO:
            return "\033[35m";
        case CIANO_ESCURO:
            return "\033[36m";
        case BRANCO_ESCURO:
            return "\033[37m";
        default:
            return "";
        }
    } else if (tipo == 1) {
        switch (cor) {
        case PADRAO:
            return "\033[0m";
        case PRETO_CLARO:
            return "\033[100m";
        case VERMELHO_CLARO:
            return "\033[101m";
        case VERDE_CLARO:
            return "\033[102m";
        case AMARELO_CLARO:
            return "\033[103m";
        case AZUL_CLARO:
            return "\033[104m";
        case MAGENTA_CLARO:
            return "\033[105m";
        case CIANO_CLARO:
            return "\033[106m";
        case BRANCO_CLARO:
            return "\033[107m";
        case PRETO_ESCURO:
            return "\033[40m";
        case VERMELHO_ESCURO:
            return "\033[41m";
        case VERDE_ESCURO:
            return "\033[42m";
        case AMARELO_ESCURO:
            return "\033[43m";
        case AZUL_ESCURO:
            return "\033[44m";
        case MAGENTA_ESCURO:
            return "\033[45m";
        case CIANO_ESCURO:
            return "\033[46m";
        case BRANCO_ESCURO:
            return "\033[47m";
        default:
            return "";
        }
    }
    return "";
}

/**
 * @name processar_string_cli
 * @return void
 * @param str:char*
 *
 * Função que processa uma string contendo sequências de escape ANSI e processa
 *comandos de cores, imprimindo caracteres normais ou aplicando as cores
 *configuradas.
 **/
void processar_string_cli(char *str) {
    char *ptr = str;          /* Ponteiro para a string de entrada */
    while (*ptr != '\0') {    /* Enquanto não atingir o fim da string */
        if (*ptr == '\033') { /* Início de uma sequência de escape */
            char *esc_inicio = ptr;
            ptr++;             /* Pula o '\033' */
            if (*ptr == '[') { /* Se for sequência de escape ANSI */
                ptr++;         /* Pula o '[' */
                int resetar_cores =
                    0; /* Flag para verificar se deve resetar as cores */
                int cor_fg = -1,
                    cor_bg = -1; /* Cores de primeiro plano (fg) e fundo (bg) */

                /* Processa os valores na sequência de escape */
                while (isdigit(*ptr) || *ptr == ';') {
                    int valor = 0;
                    while (isdigit(*ptr)) { /* Lê o número */
                        valor = valor * 10 + (*ptr - '0');
                        ptr++;
                    }

                    /* Verifica e ajusta as cores com base nos valores
                     * encontrados */
                    if (valor == 0) { /* Código de reset */
                        resetar_cores = 1;
                    } else if (valor >= 30 &&
                               valor <=
                                   37) { /* Cor de primeiro plano (brilho) */
                        cor_fg = valor - 30 + PRETO_ESCURO;
                    } else if (valor >= 90 &&
                               valor <=
                                   97) { /* Cor de primeiro plano (claro) */
                        cor_fg = valor - 90 + PRETO_CLARO;
                    } else if (valor >= 40 &&
                               valor <= 47) { /* Cor de fundo (brilho) */
                        cor_bg = valor - 40 + PRETO_ESCURO;
                    } else if (valor >= 100 &&
                               valor <= 107) { /* Cor de fundo (claro) */
                        cor_bg = valor - 100 + PRETO_CLARO;
                    }

                    if (*ptr == ';') {
                        ptr++; /* Pula o separador ';' */
                    }
                }

                if (*ptr == 'm') { /* Final da sequência de escape */
                    ptr++;         /* Pula o 'm' no final da sequência */
                    if (resetar_cores) {
                        cli_redefinir_cores(); /* Restaura as cores padrão */
                    } else {
                        cli_definir_cores(
                            cor_fg, cor_bg); /* Define as cores de fg e bg */
                    }
                }
            }
        } else {
            putchar(*ptr); /* Imprime caracteres normais */
            ptr++;         /* Avança para o próximo caractere */
        }
    }
}

/**
 * @name duplicar_string
 * @return char*
 * @param origem:const char*
 * Função para duplicar uma string, alocando memória suficiente para armazenar
 *uma cópia da string fornecida, incluindo o caractere nulo terminador.
 **/
char *duplicar_string(const char *origem) {
    unsigned long tamanho;
    char *nova_string;

    /* Calcula o tamanho da nova string (quantidade de caracteres UTF-8 + 1 para
     * o nulo) */
    tamanho = (contar_caracteres_utf8(origem) * 2) + 1;

    /* Aloca memória suficiente para armazenar a nova string */
    nova_string = malloc(tamanho);
    if (nova_string == NULL) {
        /* Se a alocação falhar, imprime a mensagem de erro e termina o programa
         */
        fprintf(stderr,
                "Erro de alocação de memória na duplicação de string\n");
        exit(1);
    }

    /* Copia o conteúdo da string original para a nova string */
    strcpy(nova_string, origem);

    return nova_string; /* Retorna a nova string alocada */
}

/**
 * @name inicio_sequencia_escape
 * @return int (retorna 1 se a sequência de escape começar, caso contrário,
 *retorna 0)
 * @param str: const char* (ponteiro para a string a ser analisada)
 * @param i: int (índice da posição na string onde a análise começa)
 *
 * A função verifica se, na posição i da string fornecida, há o início de uma
 *sequência de escape ANSI. A sequência de escape começa com o caractere '\033'
 *(ESC) seguido por o caractere '['.
 **/
int inicio_sequencia_escape(const char *str, int i) {
    /* Verifica se o caractere na posição i é '\033' (ESC) e o próximo caractere
     * é '['. */
    return str[i] == '\033' && str[i + 1] == '[';
}

/**
 * @name comprimento_sequencia_escape
 * @return int (retorna o comprimento da sequência de escape, ou 0 se não for
 *uma sequência de escape)
 * @param str: const char* (ponteiro para a string a ser analisada)
 * @param inicio: int (índice da posição na string onde a análise começa)
 *
 * A função calcula o comprimento de uma sequência de escape ANSI, que começa
 *com '\033[' e termina com uma letra. Se a string não começar com uma sequência
 *de escape válida, o comprimento retornado será 0.
 **/
int comprimento_sequencia_escape(const char *str, int inicio) {
    int comprimento = 0;

    /* Verifica se a posição inicial contém o início de uma sequência de escape
     */
    if (inicio_sequencia_escape(str, inicio)) {
        comprimento = 2; /* '\033[' */
        inicio += 2;     /* Avança para o próximo caractere após '\033[' */

        /* Conta o comprimento da sequência até encontrar um caractere
         * alfabético (final da sequência) */
        while (str[inicio] && !isalpha(str[inicio])) {
            comprimento++;
            inicio++;
        }

        /* Conta a letra final da sequência de escape */
        comprimento++;
    }

    return comprimento;
}

/**
 * @name dividir_linha
 * @return void
 * @param linha: const char* (ponteiro para a linha de texto a ser dividida)
 * @param resultado: char*** (ponteiro para o ponteiro de resultados, que
 *armazenará as linhas divididas)
 * @param contador_resultado: int* (ponteiro para a variável que armazenará a
 *quantidade de partes divididas)
 *
 * A função divide uma linha em partes com no máximo 80 caracteres, considerando
 *espaços e sequências de escape. Cada parte é alocada dinamicamente e
 *armazenada no vetor resultado.
 **/
void dividir_linha(const char *linha, char ***resultado,
                   int *contador_resultado) {
    /* Declaramos as variáveis antes de inicializá-las, como exige o C89. */
    int comprimento = contar_caracteres_utf8(linha);
    int contador_visivel = 0, inicio = 0, ultimo_espaco = -1;
    int i = 0;
    *contador_resultado = 0;
    *resultado = NULL;

    /* Percorre a linha caractere por caractere */
    while (i < comprimento) {
        if (inicio_sequencia_escape(linha, i)) {
            /* Ignora a sequência de escape */
            int comprimento_esc = comprimento_sequencia_escape(linha, i);
            i += comprimento_esc; /* Pula a sequência de escape */
        } else if (linha[i] == ' ') {
            /* Encontrou um espaço, armazena a posição do último espaço */
            ultimo_espaco = i;
            contador_visivel++;
            i++;
        } else {
            /* Conta os caracteres visíveis */
            contador_visivel++;
            i++;
        }

        /* Se o número de caracteres visíveis ultrapassar 80, divide a linha */
        if (contador_visivel > 80) {
            int dividir_em = (ultimo_espaco != -1) ? ultimo_espaco : i - 1;
            int comprimento_pedaço = dividir_em - inicio + 1;
            char *pedaço = (char *)malloc(comprimento_pedaço +
                                          2); /* +2 para '\n' e '\0' */
            strncpy(pedaço, linha + inicio, comprimento_pedaço);
            pedaço[comprimento_pedaço] = '\0';

            /* Adiciona '\n' se não estiver presente */
            if (pedaço[comprimento_pedaço - 1] != '\n') {
                strcat(pedaço, "\n");
            }

            /* Realoção de memória para armazenar o novo pedaço */
            *resultado = (char **)realloc(
                *resultado, (*contador_resultado + 1) * sizeof(char *));
            (*resultado)[*contador_resultado] = pedaço;
            (*contador_resultado)++;

            /* Atualiza a posição inicial para o próximo pedaço */
            inicio = dividir_em + 1;
            contador_visivel = i - inicio;
            ultimo_espaco = -1;
        }
    }

    /* Caso reste algum pedaço da linha para ser adicionado */
    if (inicio < comprimento) {
        int comprimento_pedaço = comprimento - inicio;
        char *pedaço =
            (char *)malloc(comprimento_pedaço + 2); /* +2 para '\n' e '\0' */
        strncpy(pedaço, linha + inicio, comprimento_pedaço);
        pedaço[comprimento_pedaço] = '\0';

        /* Realoção de memória para armazenar o último pedaço */
        *resultado = (char **)realloc(*resultado, (*contador_resultado + 1) *
                                                      sizeof(char *));
        (*resultado)[*contador_resultado] = pedaço;
        (*contador_resultado)++;
    }
}

/**
 * @name dividir_linhas
 * @return char** (retorna um ponteiro para um vetor de linhas divididas)
 * @param entrada: char** (ponteiro para o vetor de linhas de entrada)
 * @param quantidade_entrada: int (número de linhas de entrada)
 * @param quantidade_saida: int* (ponteiro para a variável que armazenará o
 *número de linhas divididas)
 *
 * A função divide cada linha do vetor de entrada em partes menores e armazena
 *essas partes em um novo vetor de strings. O número total de linhas divididas é
 *retornado através do ponteiro quantidade_saida.
 **/
char **dividir_linhas(char **entrada, int quantidade_entrada,
                      int *quantidade_saida) {
    /* Declara as variáveis antes de inicializá-las, conforme o padrão C89. */
    char **resultado = NULL;
    unsigned long resultado_tamanho =
        0; /* Substitui size_t por unsigned long, conforme C89 */

    /* Processa cada linha de entrada */
    for (int i = 0; i < quantidade_entrada; i++) {
        char **resultado_dividido = NULL;
        int quantidade_dividida = 0;

        /* Divide a linha atual usando a função dividir_linha */
        dividir_linha(entrada[i], &resultado_dividido, &quantidade_dividida);

        /* Realoca memória para armazenar as novas linhas divididas */
        resultado = (char **)realloc(resultado,
                                     (resultado_tamanho + quantidade_dividida) *
                                         sizeof(char *));

        /* Copia as linhas divididas para o vetor resultado */
        for (int j = 0; j < quantidade_dividida; j++) {
            resultado[resultado_tamanho++] = resultado_dividido[j];
        }

        /* Libera a memória do vetor temporário de linhas divididas */
        free(resultado_dividido);
    }

    /* Armazena a quantidade total de linhas divididas */
    *quantidade_saida = resultado_tamanho;

    /* Retorna o ponteiro para o vetor de resultados */
    return resultado;
}

/**
 * @name dividir_string_em_array
 * @return char **: Ponteiro para o array de strings resultante.
 * @param entrada: char*: String de entrada a ser dividida.
 * @param quantidade: int*: Ponteiro para armazenar a quantidade de linhas.
 * Função que divide uma string em um array de strings, usando a quebra de linha
 *'\n' como delimitador.
 **/
char **dividir_string_em_array(char *entrada, int *quantidade) {
    char **resultado = NULL;      /* Array de strings */
    int capacidade = 0;           /* Capacidade atual do array */
    int contador_linhas = 0;      /* Número de linhas encontradas */
    const char *inicio = entrada; /* Ponteiro para o início de uma substring */

    /* Aloca memória inicial para o array de strings */
    resultado = (char **)calloc(512, sizeof(char *));

    /* Loop que percorre cada caractere da string de entrada */
    while (*entrada != '\0') {
        if (*entrada == '\n') {
            /* Aloca mais espaço para o array, se necessário */
            if (contador_linhas >= capacidade) {
                capacidade = (capacidade == 0) ? 1 : capacidade * 2;
                resultado =
                    (char **)realloc(resultado, capacidade * sizeof(char *));
                if (resultado == NULL) {
                    perror("Falha ao realocar memória");
                    exit(EXIT_FAILURE); /* Se falhar ao realocar, encerra o
                                           programa */
                }
            }

            /* Aloca espaço para a linha atual e copia o conteúdo */
            int comprimento = entrada - inicio;
            resultado[contador_linhas] = (char *)malloc(
                (comprimento + 2) * sizeof(char)); /* +2 para '\n' e '\0' */
            if (resultado[contador_linhas] == NULL) {
                perror("Falha ao alocar memória");
                exit(EXIT_FAILURE);
            }
            strncpy(resultado[contador_linhas], inicio, comprimento);
            resultado[contador_linhas][comprimento] =
                '\n'; /* Adiciona o '\n' no final */
            resultado[contador_linhas][comprimento + 1] =
                '\0'; /* Finaliza a string com '\0' */
            contador_linhas++;

            inicio = entrada +
                     1; /* Move o ponteiro de início para o próximo caractere */
        }
        entrada++;
    }

    /* Processa a última linha, caso não termine com um '\n' */
    if (inicio != entrada) {
        if (contador_linhas >= capacidade) {
            capacidade = (capacidade == 0) ? 1 : capacidade * 2;
            resultado =
                (char **)realloc(resultado, capacidade * sizeof(char *));
            if (resultado == NULL) {
                perror("Falha ao realocar memória");
                exit(EXIT_FAILURE);
            }
        }

        int comprimento = entrada - inicio;
        resultado[contador_linhas] = (char *)malloc(
            (comprimento + 2) * sizeof(char)); /* +2 para '\n' e '\0' */
        if (resultado[contador_linhas] == NULL) {
            perror("Falha ao alocar memória");
            exit(EXIT_FAILURE);
        }
        strncpy(resultado[contador_linhas], inicio, comprimento);
        resultado[contador_linhas][comprimento] =
            '\n'; /* Adiciona o '\n' no final */
        resultado[contador_linhas][comprimento + 1] =
            '\0'; /* Finaliza a string com '\0' */
        contador_linhas++;
    }

    /* Armazena a quantidade de linhas no ponteiro fornecido */
    *quantidade = contador_linhas;

    return resultado;
}

/**
 * @name copiar_strings_estaticas_para_dinamicas
 * @return char** (retorna um ponteiro para o vetor de strings dinâmicas)
 * @param strings_estaticas: char** (ponteiro para o vetor de strings estáticas)
 * @param quantidade: int (número de strings no vetor estático)
 *
 * A função aloca memória dinamicamente para um vetor de strings e copia as
 *strings do vetor estático para o vetor dinâmico. Se ocorrer um erro durante a
 *alocação de memória, a função imprime uma mensagem de erro e encerra o
 *programa.
 **/
char **copiar_strings_estaticas_para_dinamicas(char *strings_estaticas[],
                                               int quantidade) {
    /* Declarar a variável de ponteiro para strings dinâmicas */
    char **strings_dinamicas = (char **)calloc(quantidade, sizeof(char *));

    /* Verifica se a alocação de memória foi bem-sucedida */
    if (!strings_dinamicas) {
        fprintf(stderr, "Erro ao alocar memória para o vetor dinâmico.\n");
        exit(EXIT_FAILURE);
    }

    /* Copia cada string do vetor estático para o vetor dinâmico */
    for (int i = 0; i < quantidade; i++) {
        /* Usa uma função auxiliar para duplicar a string (supondo que a função
         * duplicar_string seja implementada) */
        strings_dinamicas[i] = duplicar_string(strings_estaticas[i]);

        /* Verifica se a duplicação da string foi bem-sucedida */
        if (!strings_dinamicas[i]) {
            fprintf(stderr, "Erro ao alocar memória para a cópia da string.\n");

            /* Libera as strings já copiadas, caso haja falha */
            for (int j = 0; j < i; j++) {
                free(strings_dinamicas[j]);
            }

            /* Libera o vetor dinâmico antes de encerrar o programa */
            free(strings_dinamicas);
            exit(EXIT_FAILURE);
        }
    }

    /* Retorna o ponteiro para o vetor de strings dinâmicas */
    return strings_dinamicas;
}

/**
 * @name liberar_strings_dinamicas
 * @return void (não retorna nenhum valor)
 * @param strings: char** (ponteiro para o vetor de strings dinâmicas)
 * @param quantidade: int (número de strings no vetor dinâmico)
 *
 * A função libera a memória alocada para cada string no vetor dinâmico, e por
 *fim, libera a memória do vetor de ponteiros.
 **/
void liberar_strings_dinamicas(char **strings, int quantidade) {
    /* Libera cada string individualmente */
    for (int i = 0; i < quantidade; i++) {
        free(strings[i]);
    }

    /* Libera o vetor de ponteiros */
    free(strings);
}

/**
 * @name substituir_str
 * @return char* (retorna a nova string com as substituições feitas)
 * @param str: char* (ponteiro para a string original)
 * @param velho_sub: char* (ponteiro para a substring a ser substituída)
 * @param novo_sub: char* (ponteiro para a nova substring)
 *
 * A função substitui todas as ocorrências de uma substring na string original
 *por uma nova substring. Se alguma das entradas for inválida (NULL), a função
 *retorna NULL.
 **/
char *substituir_str(char *str, char *velho_sub, char *novo_sub) {
    /* Declarações de variáveis locais */
    char *temp, *resultado, *atual, *destino;
    unsigned long comprimento_str, comprimento_velho_sub, comprimento_novo_sub;
    unsigned long contador, comprimento_segmento, comprimento_resultado;

    /* Validação das entradas. Se algum argumento for NULL, retorna NULL. */
    if (str == NULL || velho_sub == NULL || novo_sub == NULL) {
        return NULL; /* Entrada inválida. */
    }

    /* Se o velho_sub for uma string vazia, não faz sentido realizar a
     * substituição. */
    if (*velho_sub == '\0') {
        return NULL; /* Substring vazia para substituir. */
    }

    /* Inicialização das variáveis com os comprimentos das strings. */
    comprimento_str = contar_caracteres_utf8(str);
    comprimento_velho_sub = contar_caracteres_utf8(velho_sub);
    comprimento_novo_sub = contar_caracteres_utf8(novo_sub);
    contador = 0;

    /* Contar as ocorrências de velho_sub na string str. */
    temp = str;
    while ((temp = strstr(temp, velho_sub)) != NULL) {
        contador++; /* Incrementa o contador para cada ocorrência de velho_sub.
                     */
        temp += comprimento_velho_sub; /* Avança para após a ocorrência de
                                          velho_sub. */
    }

    /* Calcular o comprimento da string resultante, considerando as
     * substituições. */
    comprimento_resultado =
        comprimento_str +
        contador * (comprimento_novo_sub - comprimento_velho_sub);

    /* Alocar memória suficiente para a nova string. +1 para o terminador nulo.
     */
    resultado = (char *)malloc(comprimento_resultado + 1); /* +1 para '\0' */
    if (resultado == NULL) {
        return NULL; /* Falha na alocação de memória. */
    }

    /* Inicializar os ponteiros para processar a string. */
    atual = str;
    destino = resultado;

    /* Substituir todas as ocorrências de velho_sub por novo_sub. */
    while ((temp = strstr(atual, velho_sub)) != NULL) {
        /* Calcular o comprimento do segmento antes da ocorrência de velho_sub.
         */
        comprimento_segmento = (unsigned long)(temp - atual);

        /* Copiar o segmento da string original até velho_sub. */
        memcpy(destino, atual, comprimento_segmento);
        destino += comprimento_segmento;

        /* Copiar o novo_sub no lugar de velho_sub. */
        memcpy(destino, novo_sub, comprimento_novo_sub);
        destino += comprimento_novo_sub;

        /* Avançar o ponteiro 'atual' para além de velho_sub. */
        atual = temp + comprimento_velho_sub;
    }

    /* Copiar o restante da string original que não foi substituída. */
    strcpy(destino, atual);

    return resultado; /* Retorna a nova string com as substituições feitas. */
}

/**
 * @name renderizar_token_debug
 * @return void (não retorna nada)
 * @param token: token_markdown_t (estrutura que contém as informações sobre o
 *token)
 *
 * A função renderiza o token de depuração, substituindo as ocorrências de '\n'
 *por '\\n' no conteúdo do token e exibindo o tipo e conteúdo do token com
 *formatação apropriada para depuração.
 **/
void renderizar_token_debug(token_markdown_t token) {
    char *texto_modificado;

    /* Verifica o tipo do token e renderiza o conteúdo correspondente. */
    switch (token.tipo) {
    case CABECALHO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s CABECALHO>\n", texto_modificado);
        break;
    case ITALICO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s ITALICO>\n", texto_modificado);
        break;
    case NEGRITO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s NEGRITO>\n", texto_modificado);
        break;
    case CODIGO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s CODIGO>\n", texto_modificado);
        break;
    case BLOCO_CITACAO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s BLOCO_CITACAO>\n", texto_modificado);
        break;
    case LISTA:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<[%d] %s LISTA>\n", token.nivel_lista, texto_modificado);
        break;
    case BLOCO_CODIGO:
        printf("<%s BLOCO_CODIGO[%s]>", token.conteudo, token.language);
        break;
    case TEXTO:
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s TEXTO>\n", texto_modificado);
        break;
    }
}

/**
 * @name renderizar_tokens_debug
 * @return void (não retorna nada)
 * @param tokens: token_markdown_t[] (array de tokens a serem renderizados)
 * @param quantidade_tokens: int (quantidade total de tokens no array)
 *
 * A função itera sobre todos os tokens fornecidos e renderiza cada um deles
 *utilizando a função renderizar_token_debug.
 **/
void renderizar_tokens_debug(token_markdown_t tokens[], int quantidade_tokens) {
    /* Itera sobre o array de tokens e chama a função renderizar_token_debug
     * para cada token. */
    int i;

    for (i = 0; i < quantidade_tokens; i++) {
        renderizar_token_debug(tokens[i]);
    }
}

/**
 * @name iniciar_com_operador
 * @return int
 * @param caractere:char
 * Verifica se o caractere fornecido é um dos operadores definidos. Retorna 1
 *(verdadeiro) se for operador e 0 (falso) caso contrário.
 **/
int iniciar_com_operador(char c) {
    return strchr("+-*/%=&|^~<>!?:.", c) != NULL;
}

/**
 * @name encontrar_operador
 * @return const char*
 * @param ponteiro:const char*
 * Procura por um operador específico em um ponteiro de caracteres.
 * Retorna um ponteiro para o operador encontrado ou NULL caso não encontre.
 **/
const char *encontrar_operador(const char *ponteiro) {
    /* Declaração de variáveis */
    int indice;
    unsigned long comprimento;
    const char *resultado;

    /* Inicialização de variáveis */
    indice = 0;
    comprimento = 0;
    resultado = NULL;

    /* Itera sobre os operadores pré-definidos */
    for (indice = 0; operadores[indice] != NULL; indice++) {
        /* Obtém o comprimento do operador atual em UTF-8 */
        comprimento = contar_caracteres_utf8(operadores[indice]);

        /* Compara a string no ponteiro com o operador atual */
        if (strncmp(ponteiro, operadores[indice], comprimento) == 0) {
            /* Retorna o operador encontrado */
            resultado = operadores[indice];
            return resultado;
        }
    }

    /* Retorna NULL caso nenhum operador seja encontrado */
    return NULL;
}

/**
 * @name encontrar_palavra_chave
 * @return int
 * @param palavra:const char*
 * Verifica se uma palavra fornecida corresponde a alguma das palavras-chave
 *predefinidas. Retorna 1 (verdadeiro) se encontrar a palavra-chave, ou 0
 *(falso) caso contrário.
 **/
int encontrar_palavra_chave(const char *palavra) {
    /* Declaração de variáveis */
    int indice;

    /* Inicialização de variáveis */
    indice = 0;

    /* Itera sobre o array de palavras-chave */
    for (indice = 0; palavras_chave[indice] != NULL; indice++) {
        /* Compara a palavra fornecida com a palavra-chave atual */
        if (strcmp(palavra, palavras_chave[indice]) == 0) {
            /* Retorna verdadeiro se a palavra for encontrada */
            return 1;
        }
    }

    /* Retorna falso se nenhuma correspondência for encontrada */
    return 0;
}

/**
 * @name colorizar_codigo_em_c
 * @return void
 * @param codigo:const char*
 * Realiza a colorização sintática de um código fonte em C, utilizando códigos
 *ANSI para formatar o texto no terminal. Lida com diferentes elementos da
 *linguagem, como comentários, diretivas do pré-processador, strings e
 *operadores.
 **/
void colorizar_codigo_em_c(const char *codigo) {
    /* Declaração de variáveis */
    const char *ponteiro;
    char buffer[256];
    int indice;
    int colunas;

    /* Inicialização de variáveis */
    ponteiro = codigo;
    colunas = 0;

    /* Itera sobre cada caractere no código fornecido */
    while (*ponteiro != '\0') {
        /* Lida com comentários */
        if (*ponteiro == '/' && *(ponteiro + 1) == '*') {
            printf("%s/*", obter_codigo_ansi(CIANO_CLARO, 0));
            ponteiro += 2;
            colunas += 2;

            while (*ponteiro != '\0' &&
                   !(*ponteiro == '*' && *(ponteiro + 1) == '/')) {
                colunas++;
                if (colunas == 80) {
                    putchar('\n');
                    colunas = 0;
                }
                putchar(*ponteiro++);
            }

            if (*ponteiro == '*' && *(ponteiro + 1) == '/') {
                printf("*/%s", obter_codigo_ansi(PADRAO, 0));
                ponteiro += 2;
            }
            colunas = 0;
            continue;
        }

        /* Lida com diretivas do pré-processador */
        if (*ponteiro == '#' &&
            (ponteiro == codigo || *(ponteiro - 1) == '\n')) {
            if (strncmp(ponteiro, "#include", 8) == 0 &&
                isspace(*(ponteiro + 8))) {
                printf("%s#include", obter_codigo_ansi(VERMELHO_CLARO, 0));
                ponteiro += 8;

                while (isspace(*ponteiro)) {
                    putchar(*ponteiro++);
                }

                if (*ponteiro == '<') {
                    printf("%s<", obter_codigo_ansi(CIANO_CLARO, 0));
                    ponteiro++;
                    while (*ponteiro != '\0' && *ponteiro != '>') {
                        putchar(*ponteiro++);
                    }
                    if (*ponteiro == '>') {
                        printf(">%s", obter_codigo_ansi(PADRAO, 0));
                        ponteiro++;
                    }
                    continue;
                }

                if (*ponteiro == '"') {
                    printf("%s\"", obter_codigo_ansi(MAGENTA_CLARO, 0));
                    ponteiro++;
                    while (*ponteiro != '\0' && *ponteiro != '"') {
                        putchar(*ponteiro++);
                    }
                    if (*ponteiro == '"') {
                        printf("\"%s", obter_codigo_ansi(PADRAO, 0));
                        ponteiro++;
                    }
                    continue;
                }
            } else {
                printf("%s#", obter_codigo_ansi(VERMELHO_CLARO, 0));
                ponteiro++;
                while (*ponteiro != '\0' && *ponteiro != '\n' &&
                       !(*ponteiro == '/' && *(ponteiro + 1) == '*')) {
                    putchar(*ponteiro++);
                }
                printf(obter_codigo_ansi(PADRAO, 0));
                continue;
            }
        }

        /* Lida com caracteres entre aspas simples */
        if (*ponteiro == '\'') {
            printf("%s'", obter_codigo_ansi(CIANO_CLARO, 0));
            ponteiro++;
            if (*ponteiro == '\\') {
                putchar(*ponteiro++);
                if (*ponteiro != '\0') {
                    putchar(*ponteiro++);
                }
            } else if (*ponteiro != '\0') {
                putchar(*ponteiro++);
            }
            if (*ponteiro == '\'') {
                printf("'%s", obter_codigo_ansi(PADRAO, 0));
                ponteiro++;
            }
            continue;
        }

        /* Lida com strings entre aspas duplas */
        if (*ponteiro == '"') {
            printf("%s\"", obter_codigo_ansi(VERDE_CLARO, 0));
            ponteiro++;
            while (*ponteiro != '\0' && *ponteiro != '"') {
                if (*ponteiro == '\\' && *(ponteiro + 1) == '"') {
                    printf("\\\"");
                    ponteiro += 2;
                } else {
                    putchar(*ponteiro++);
                }
            }
            if (*ponteiro == '"') {
                printf("\"%s", obter_codigo_ansi(PADRAO, 0));
                ponteiro++;
            }
            continue;
        }

        /* Lida com números */
        if (isdigit(*ponteiro) ||
            (*ponteiro == '.' && isdigit(*(ponteiro + 1)))) {
            printf(obter_codigo_ansi(MAGENTA_CLARO, 0));
            while (isdigit(*ponteiro) || *ponteiro == '.') {
                putchar(*ponteiro++);
            }
            printf(obter_codigo_ansi(PADRAO, 0));
            continue;
        }

        /* Lida com operadores */
        if (iniciar_com_operador(*ponteiro)) {
            const char *operador = encontrar_operador(ponteiro);
            if (operador != NULL) {
                printf("%s%s%s", obter_codigo_ansi(AMARELO_CLARO, 0), operador,
                       obter_codigo_ansi(PADRAO, 0));
                ponteiro += contar_caracteres_utf8(operador);
                continue;
            }
        }

        /* Lida com palavras-chave e identificadores */
        if (isalpha(*ponteiro) || *ponteiro == '_') {
            indice = 0;
            while (isalnum(*ponteiro) || *ponteiro == '_') {
                buffer[indice++] = *ponteiro++;
            }
            buffer[indice] = '\0';
            if (encontrar_palavra_chave(buffer)) {
                printf("%s%s%s", obter_codigo_ansi(AZUL_CLARO, 0), buffer,
                       obter_codigo_ansi(PADRAO, 0));
            } else {
                printf("%s", buffer);
            }
            continue;
        }

        /* Imprime outros caracteres */
        putchar(*ponteiro++);
    }
}

/**
 * @name colorizar_codigo_em_c_array_strings
 * @return char**
 * @param codigo:char* String contendo o código-fonte a ser colorizado.
 * @param num_linhas:unsigned long* Ponteiro para armazenar o número de linhas
 *colorizadas geradas. A função percorre o código em formato de string,
 *aplicando colorização semântica (baseada em palavras-chave, números,
 *operadores, etc.) e organiza as linhas colorizadas em um vetor de strings.
 **/
char **colorizar_codigo_em_c_array_strings(char *codigo,
                                           unsigned long *num_linhas) {
    char *ponteiro;
    char *comentario_inicio;
    char *comentario_final;
    char *preprocessador_include_inicio;
    char *include_global_inicio;
    char *include_local_inicio;
    char *preprocessador_inicio;
    char *string_sq_inicio;
    char *string_sq_final;
    char *string_dq_inicio;
    char *string_dq_final;
    char buffer[256];
    unsigned long capacidade_linhas;
    unsigned long tamanho_linha;
    unsigned long capacidade_linha;
    char **linhas;
    char *linha_atual;

    /* Inicializa os ponteiros e variáveis. */
    ponteiro = codigo;
    capacidade_linhas = 10;
    *num_linhas = 0;

    /* Aloca memória para armazenar as linhas do código e a linha atual. */
    linhas = malloc(capacidade_linhas * sizeof(char *));
    linha_atual = malloc(256);
    tamanho_linha = 0;
    capacidade_linha = 256;

    /* Verifica se a alocação de memória falhou. */
    if (!linhas || !linha_atual) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }

    linha_atual[0] = '\0';

    while (*ponteiro != '\0') {
        /* Lida com comentários */
        if (*ponteiro == '/' && *(ponteiro + 1) == '*') {
            comentario_inicio =
                printf_para_string("%s/*", obter_codigo_ansi(CIANO_CLARO, 0));
            strcat(linha_atual, comentario_inicio);
            free(comentario_inicio);
            ponteiro += 2;
            tamanho_linha += 4;
            while (*ponteiro != '\0' &&
                   !(*ponteiro == '*' && *(ponteiro + 1) == '/')) {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            if (*ponteiro == '*' && *(ponteiro + 1) == '/') {
                comentario_final =
                    printf_para_string("*/%s", obter_codigo_ansi(PADRAO, 0));
                strcat(linha_atual, comentario_final);
                free(comentario_final);
                ponteiro += 2;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com diretivas do pré-processador */
        if (*ponteiro == '#' &&
            (ponteiro == codigo || *(ponteiro - 1) == '\n')) {
            if (strncmp(ponteiro, "#include", 8) == 0 &&
                isspace(*(ponteiro + 8))) {
                preprocessador_include_inicio = printf_para_string(
                    "%s#include", obter_codigo_ansi(VERMELHO_CLARO, 0));
                strcat(linha_atual, preprocessador_include_inicio);
                free(preprocessador_include_inicio);
                ponteiro += 8;

                /* Ignora espaços em branco */
                while (isspace(*ponteiro)) {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }

                /* Lida com <...> */
                if (*ponteiro == '<') {
                    include_global_inicio = printf_para_string(
                        "%s<", obter_codigo_ansi(CIANO_CLARO, 0));
                    strcat(linha_atual, include_global_inicio);
                    free(include_global_inicio);
                    ponteiro++;
                    tamanho_linha++;
                    while (*ponteiro != '\0' && *ponteiro != '>') {
                        strncat(linha_atual, ponteiro++, 1);
                        tamanho_linha++;
                    }
                    if (*ponteiro == '>') {
                        strncat(linha_atual, ponteiro++, 1);
                        strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
                        tamanho_linha += 2;
                    }
                    continue;
                }

                /* Lida com "..." */
                if (*ponteiro == '"') {
                    include_local_inicio = printf_para_string(
                        "%s\"", obter_codigo_ansi(MAGENTA_CLARO, 0));
                    strcat(linha_atual, include_local_inicio);
                    free(include_local_inicio);
                    ponteiro++;
                    tamanho_linha++;
                    while (*ponteiro != '\0' && *ponteiro != '"') {
                        strncat(linha_atual, ponteiro++, 1);
                        tamanho_linha++;
                    }
                    if (*ponteiro == '"') {
                        strncat(linha_atual, ponteiro++, 1);
                        strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
                        tamanho_linha++;
                    }
                    continue;
                }
            } else {
                preprocessador_inicio = printf_para_string(
                    "%s#", obter_codigo_ansi(VERMELHO_CLARO, 0));
                strcat(linha_atual, preprocessador_inicio);
                free(preprocessador_inicio);
                ponteiro++;
                tamanho_linha++;
                while (*ponteiro != '\0' && *ponteiro != '\n' &&
                       !(*ponteiro == '/' && *(ponteiro + 1) == '*')) {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }
                strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
                continue;
            }
        }

        /* Lida com caracteres entre aspas simples */
        if (*ponteiro == '\'') {
            string_sq_inicio =
                printf_para_string("%s'", obter_codigo_ansi(CIANO_CLARO, 0));
            strcat(linha_atual, string_sq_inicio);
            free(string_sq_inicio);
            ponteiro++;
            tamanho_linha++;
            if (*ponteiro == '\\') {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
                if (*ponteiro != '\0') {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }
            } else if (*ponteiro != '\0') {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            if (*ponteiro == '\'') {
                string_sq_final =
                    printf_para_string("'%s", obter_codigo_ansi(PADRAO, 0));
                strcat(linha_atual, string_sq_final);
                free(string_sq_final);
                ponteiro++;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com strings entre aspas duplas */
        if (*ponteiro == '"') {
            string_dq_inicio =
                printf_para_string("%s\"", obter_codigo_ansi(VERDE_CLARO, 0));
            strcat(linha_atual, string_dq_inicio);
            free(string_dq_inicio);
            ponteiro++;
            tamanho_linha++;
            while (*ponteiro != '\0' && *ponteiro != '"') {
                if (*ponteiro == '\\' && *(ponteiro + 1) == '"') {
                    strcat(linha_atual, "\\\"");
                    ponteiro += 2;
                    tamanho_linha += 2;
                } else {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }
            }
            if (*ponteiro == '"') {
                string_dq_final =
                    printf_para_string("\"%s", obter_codigo_ansi(PADRAO, 0));
                strcat(linha_atual, string_dq_final);
                free(string_dq_final);
                ponteiro++;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com números */
        if (isdigit(*ponteiro) ||
            (*ponteiro == '.' && isdigit(*(ponteiro + 1)))) {
            strcat(linha_atual, obter_codigo_ansi(MAGENTA_CLARO, 0));
            tamanho_linha +=
                contar_caracteres_utf8(obter_codigo_ansi(MAGENTA_CLARO, 0));
            while (isdigit(*ponteiro) || *ponteiro == '.') {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
            tamanho_linha +=
                contar_caracteres_utf8(obter_codigo_ansi(PADRAO, 0));
            continue;
        }

        /* Lida com operadores */
        if (iniciar_com_operador(*ponteiro)) {
            const char *op;
            op = encontrar_operador(ponteiro);
            if (op != NULL) {
                strcat(linha_atual, obter_codigo_ansi(AMARELO_CLARO, 0));
                strcat(linha_atual, op);
                strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
                ponteiro += contar_caracteres_utf8(op);
                tamanho_linha +=
                    contar_caracteres_utf8(op) +
                    contar_caracteres_utf8(
                        obter_codigo_ansi(AMARELO_CLARO, 0)) +
                    contar_caracteres_utf8(obter_codigo_ansi(PADRAO, 0));
                continue;
            }
        }

        /* Lida com palavras-chave e identificadores */
        if (isalpha(*ponteiro) || *ponteiro == '_') {
            unsigned long i;
            i = 0;
            while (isalnum(*ponteiro) || *ponteiro == '_') {
                buffer[i++] = *ponteiro++;
            }
            buffer[i] = '\0';
            if (encontrar_palavra_chave(buffer)) {
                strcat(linha_atual, obter_codigo_ansi(AZUL_CLARO, 0));
                strcat(linha_atual, buffer);
                strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
                tamanho_linha +=
                    contar_caracteres_utf8(buffer) +
                    contar_caracteres_utf8(obter_codigo_ansi(AZUL_CLARO, 0)) +
                    contar_caracteres_utf8(obter_codigo_ansi(PADRAO, 0));
            } else {
                strcat(linha_atual, buffer);
                tamanho_linha += contar_caracteres_utf8(buffer);
            }
            continue;
        }

        /* Lida com novas linhas */
        if (*ponteiro == '\n') {
            strncat(linha_atual, ponteiro++, 1);
            linhas[*num_linhas] = duplicar_string(linha_atual);
            (*num_linhas)++;
            if (*num_linhas >= capacidade_linhas) {
                capacidade_linhas *= 2;
                linhas = realloc(linhas, capacidade_linhas * sizeof(char *));
                if (!linhas) {
                    fprintf(stderr, "Erro de alocação de memória\n");
                    exit(1);
                }
            }
            tamanho_linha = 0;
            linha_atual[0] = '\0';
            continue;
        }

        /* Adiciona outros caracteres */
        strncat(linha_atual, ponteiro++, 1);
        tamanho_linha++;
    }

    /* Adiciona a última linha, se houver conteúdo */
    if (tamanho_linha > 0) {
        linhas[*num_linhas] = duplicar_string(linha_atual);
        (*num_linhas)++;
    }

    free(linha_atual);
    return linhas;
}

/**
 * @name concatenar_dinamico
 * @param destino Ponteiro para a string destino. Será realocado para acomodar o
 * conteúdo.
 * @param fonte String a ser concatenada.
 * @return novo_destino:char* Ponteiro para a string concatenada ou NULL em caso
 * de erro.
 *
 * Função para concatenar dinamicamente strings.
 */
char *concatenar_dinamico(char *destino, const char *fonte) {
    unsigned long tamanho_atual = destino ? strlen(destino) : 0;
    unsigned long tamanho_fonte = fonte ? strlen(fonte) : 0;
    char *novo_destino = realloc(destino, tamanho_atual + tamanho_fonte + 1);
    if (novo_destino == NULL) {
        free(destino); // Libera memória anterior, se houver
        return NULL;
    }
    strcpy(novo_destino + tamanho_atual, fonte);
    return novo_destino;
}

/**
 * @name remover_ultima_nova_linha
 * @param str A string de entrada a ser modificada.
 * @return str:char* Retorna a string modificada ou NULL se a entrada for
 * inválida.
 *
 * Remove o último '\n' no final de uma string, se existir.
 */
char *remover_ultima_nova_linha(char *str) {
    int i, j;

    /* Verifica se a string é nula para evitar problemas */
    if (str == NULL) {
        return NULL;
    }

    /* Inicializa os índices:
     * - i: percorre os caracteres da string original.
     * - j: controla a posição para onde os caracteres válidos serão movidos.
     */
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        /* Copia o caractere para a nova posição apenas se não for '\n' */
        if (str[i] != '\n') {
            str[j++] = str[i];
        }
    }

    /* Adiciona o terminador nulo '\0' na posição final */
    str[j] = '\0';

    return str;
}

/**
 * Função para renderizar tokens Markdown em uma sequência de strings
 * formatadas.
 *
 * @param tokens: Array de tokens a serem processados.
 * @param quantidade_tokens: Número de tokens no array.
 * @param numero_linhas: Contador de linhas geradas.
 * @return char**: Array de strings formatadas ou NULL em caso de erro.
 */
char **renderizar_tokens_para_strings(token_markdown_t tokens[],
                                      int quantidade_tokens,
                                      int *numero_linhas) {
    int i;
    char *linha_atual = NULL;
    char *token_atual = NULL;
    char **linhas = NULL;
    char **linhas_codigo = NULL;
    unsigned long num_linhas_codigo;

    // Inicializa o contador de linhas
    *numero_linhas = 0;

    // Aloca memória para armazenar as linhas
    linhas = (char **)calloc(quantidade_tokens, sizeof(char *));
    if (linhas == NULL) {
        return NULL;
    }

    // Itera sobre os tokens
    for (i = 0; i < quantidade_tokens; i++) {
        // Libera o token anterior e reinicia o token atual
        free(token_atual);
        token_atual = NULL;

        switch (tokens[i].tipo) {
        case CABECALHO:
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(BRANCO_CLARO, 1));
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(PRETO_ESCURO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 1));
            token_atual = concatenar_dinamico(token_atual, "\n");
            break;

        case ITALICO:
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(AMARELO_ESCURO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            break;

        case NEGRITO:
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(BRANCO_CLARO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            break;

        case CODIGO:
            token_atual = concatenar_dinamico(token_atual,
                                              obter_codigo_ansi(AZUL_CLARO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            break;

        case LISTA:
            // Inicializa o token_atual com o código ANSI e conteúdo
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(BRANCO_ESCURO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));

            if (token_atual == NULL) {
                liberar_strings_dinamicas(linhas, *numero_linhas);
                return NULL;
            }

            // Cria uma string de tabulação com base no nivel_lista
            char *tabs =
                malloc(tokens[i].nivel_lista + 1); // +1 para o '\0' no final
            if (tabs == NULL) {
                free(token_atual);
                liberar_strings_dinamicas(linhas, *numero_linhas);
                return NULL;
            }

            // Preenche a string com '\t' de acordo com o nível da lista
            for (int j = 0; j < tokens[i].nivel_lista; j++) {
                tabs[j] = '\t';
            }
            tabs[tokens[i].nivel_lista] =
                '\0'; // Garantir que a string termine com '\0'

            // Adiciona os tabs no início do token_atual
            char *novo_token_atual = concatenar_dinamico(NULL, tabs);
            novo_token_atual =
                concatenar_dinamico(novo_token_atual, token_atual);
            free(tabs);
            free(token_atual);

            if (novo_token_atual == NULL) {
                liberar_strings_dinamicas(linhas, *numero_linhas);
                return NULL;
            }

            token_atual = novo_token_atual;
            break;

        case BLOCO_CITACAO:
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(PRETO_CLARO, 0));
            token_atual = concatenar_dinamico(token_atual, "> ");
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            break;

        case BLOCO_CODIGO:
            if (strcmp(tokens[i].language, "c") == 0) {
                linhas_codigo = colorizar_codigo_em_c_array_strings(
                    tokens[i].conteudo, &num_linhas_codigo);
                if (linhas_codigo == NULL) {
                    free(token_atual);
                    liberar_strings_dinamicas(linhas, *numero_linhas);
                    return NULL;
                }
                for (unsigned long j = 0; j < num_linhas_codigo; j++) {
                    linhas[*numero_linhas] = duplicar_string(linhas_codigo[j]);
                    if (linhas[*numero_linhas] == NULL) {
                        liberar_strings_dinamicas(linhas_codigo,
                                                  num_linhas_codigo);
                        liberar_strings_dinamicas(linhas, *numero_linhas);
                        return NULL;
                    }
                    (*numero_linhas)++;
                }
                liberar_strings_dinamicas(linhas_codigo, num_linhas_codigo);
                continue;
            } else {
                token_atual = concatenar_dinamico(
                    token_atual, obter_codigo_ansi(AZUL_ESCURO, 0));
                token_atual =
                    concatenar_dinamico(token_atual, tokens[i].conteudo);
                token_atual = concatenar_dinamico(token_atual,
                                                  obter_codigo_ansi(PADRAO, 0));
            }
            break;

        case TEXTO:
            token_atual = concatenar_dinamico(
                token_atual, obter_codigo_ansi(BRANCO_ESCURO, 0));
            token_atual = concatenar_dinamico(token_atual, tokens[i].conteudo);
            token_atual =
                concatenar_dinamico(token_atual, obter_codigo_ansi(PADRAO, 0));
            break;
        }

        if (token_atual == NULL) {
            liberar_strings_dinamicas(linhas, *numero_linhas);
            return NULL;
        }

        // Concatena o token à linha atual
        linha_atual = concatenar_dinamico(linha_atual, token_atual);

        // Se encontrar uma nova linha, armazena a linha atual
        if (strchr(linha_atual, '\n') != NULL) {
            linhas[*numero_linhas] = duplicar_string(linha_atual);
            if (linhas[*numero_linhas] == NULL) {
                liberar_strings_dinamicas(linhas, *numero_linhas);
                free(token_atual);
                free(linha_atual);
                return NULL;
            }
            (*numero_linhas)++;
            free(linha_atual);
            linha_atual = NULL;
        }
    }

    // Adiciona a última linha, se houver
    if (linha_atual != NULL) {
        linhas[*numero_linhas] = duplicar_string(linha_atual);
        if (linhas[*numero_linhas] == NULL) {
            liberar_strings_dinamicas(linhas, *numero_linhas);
            free(token_atual);
            free(linha_atual);
            return NULL;
        }
        (*numero_linhas)++;
        free(linha_atual);
    }

    free(token_atual);
    return linhas;
}

/**
 * @name tokenizar
 * @return void
 * @param linha:char* - Linha de texto a ser tokenizada.
 * @param tokens:token_markdown_t[] - Array de tokens a serem preenchidos.
 * @param quantidade_tokens:int* - Ponteiro para o número de tokens.
 * @param espacos_lista:int* - Ponteiro para o número de espaços da lista.
 *
 * Esta função processa uma linha de texto e a divide em tokens com base em
 *marcações específicas de markdown (como blocos de código, listas, negrito,
 *itálico, etc.).
 **/
void tokenizar(char *linha, token_markdown_t tokens[], int *quantidade_tokens,
               int *espacos_lista) {
    /* Declaração de variáveis */
    static int em_bloco_codigo = 0;
    char buffer[1024];
    int indice_buffer = 0;
    char *ptr = linha;
    char *language;
    char language_buffer[1024];
    int language_c = 0;
    char *ch;
    int identacao_lista = 0;
    int identacao_atual = 0;
    char title_buffer[1024];
    int title_c = 0;
    char bold_buffer[1024];
    int bold_c = 0;
    char italic_buffer[1024];
    int italic_c = 0;
    char codeblock_buffer[1024];
    int codeblock_c = 0;

    /* Verifica início ou fim de bloco de código */
    if (strncmp(linha, "```", 3) == 0) {
        if (em_bloco_codigo) {
            /* Fecha bloco de código */
            tokens[*quantidade_tokens].tipo = BLOCO_CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, "");
            (*quantidade_tokens)++;
            em_bloco_codigo = 0;
        } else {
            /* Inicia bloco de código */
            tokens[*quantidade_tokens].tipo = BLOCO_CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, "");
            /* Verifica se há um identificador de linguagem após os backticks */
            language = linha + 3; /* Pula os "```" */
            while (*language == ' ' || *language == '\t') {
                language++;
            }
            /* Extrai a linguagem (até encontrar espaço, tabulação ou nova
             * linha) */
            if (*language != '\0' && *language != '\n') {
                while (*language != ' ' && *language != '\t' &&
                       *language != '\n' && *language != '\0') {
                    language_buffer[language_c++] = *language++;
                }
                language_buffer[language_c] = '\0';
                /* Armazena a linguagem em um novo token (tipo LINGUAGEM_CODIGO)
                 */
                strcpy(tokens[*quantidade_tokens].language, language_buffer);
            }
            (*quantidade_tokens)++;
            em_bloco_codigo = 1;
        }
        return;
    }

    /* Trata o conteúdo do bloco de código */
    if (em_bloco_codigo) {
        /* Coleta o conteúdo dentro do bloco de código */
        strcat(tokens[*quantidade_tokens - 1].conteudo, linha);
        return;
    }

    /* Trata outros tipos de tokens */
    if (*linha == '>' && *(linha + 1) == ' ') {
        tokens[*quantidade_tokens].tipo = BLOCO_CITACAO;
        strcpy(tokens[*quantidade_tokens].conteudo, linha + 2);
        (*quantidade_tokens)++;
        return;
    }

    /* Trata itens de lista e acompanha o nível de indentação */
    if (((*linha == '-' || *linha == '*') && *(linha + 1) == ' ') ||
        (*linha == ' ') || (*linha == '\t')) {
        /* Inicializa a indentação da lista */
        identacao_lista = 0;
        identacao_atual = 0;
        ch = linha;
        while ((*ch == ' ') || (*ch == '\t')) {
            identacao_lista++;
            ch++;
        }

        /* Marca o primeiro item da lista com o número de espaços de indentação
         */
        if (*espacos_lista == 0 && identacao_lista > 0) {
            *espacos_lista =
                identacao_lista; /* Define a indentação para itens da lista */
        }

        /* Evita divisão por zero, verifica se os espaços de indentação foram
         * definidos */
        if (*espacos_lista > 0 && identacao_lista % *espacos_lista == 0) {
            int nivel_lista = identacao_lista / *espacos_lista;
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = nivel_lista;
            strcpy(tokens[*quantidade_tokens].conteudo,
                   ch); /* Armazena o conteúdo do item da lista */
            (*quantidade_tokens)++;
        } else if (*espacos_lista == 0) {
            /* Sem indentação */
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = 0;
            strcpy(tokens[*quantidade_tokens].conteudo,
                   ch); /* Armazena o conteúdo do item da lista */
            (*quantidade_tokens)++;
        }
        return;
    }

    /* Processa formatação inline (Negrito, Itálico, etc.) */
    while (*ptr) {
        if (*ptr == '#' && (ptr == linha || *(ptr - 1) == ' ')) {
            /* Processa título */
            title_c = 0;
            ptr += 2;
            title_buffer[title_c] = ' ';
            title_c++;
            while ((*ptr != '\n') && (title_c < 1024 - 1)) {
                title_buffer[title_c] = *ptr;
                ptr++;
                title_c++;
            }

            title_buffer[title_c] = ' ';
            title_c++;

            if (*ptr == '\n' && title_c < 1024 - 1) {
                title_buffer[title_c] = '\n';
                title_c++;
            }
            title_buffer[title_c] = '\0';
            tokens[*quantidade_tokens].tipo = CABECALHO;
            strcpy(tokens[*quantidade_tokens].conteudo, title_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '*' && *(ptr + 1) == '*' &&
                   indice_buffer < 1024 - 1) {
            /* Se houver texto regular antes da formatação em negrito,
             * processa-se o texto */
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; /* Finaliza o texto */
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; /* Reseta o buffer para novo conteúdo */
            }
            /* Trata formatação em negrito: coleta o texto em negrito */
            ptr += 2; /* Pula os '**' */
            bold_c = 0;
            while (!(*ptr == '*' && *(ptr + 1) == '*') && bold_c < 1024 - 1) {
                bold_buffer[bold_c] = *ptr;
                ptr++;
                bold_c++;
            }
            ptr++; /* Pula o segundo '*' */
            bold_buffer[bold_c] = '\0';
            tokens[*quantidade_tokens].tipo = NEGRITO;
            strcpy(tokens[*quantidade_tokens].conteudo, bold_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '*' || *ptr == '_') {
            /* Formatação em itálico: coleta o texto em itálico */
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; /* Finaliza o texto */
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; /* Reseta o buffer para novo conteúdo */
            }
            ptr++; /* Pula o '*' ou '_' */
            italic_c = 0;
            while (!(*ptr == '*' || *ptr == '_') && italic_c < 1024 - 1) {
                italic_buffer[italic_c] = *ptr;
                ptr++;
                italic_c++;
            }
            italic_buffer[italic_c] = '\0';
            tokens[*quantidade_tokens].tipo = ITALICO;
            strcpy(tokens[*quantidade_tokens].conteudo, italic_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '`') {
            /* Formatação de código: coleta o código inline */
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; /* Finaliza o texto */
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; /* Reseta o buffer para novo conteúdo */
            }
            ptr++; /* Pula o '`' */
            codeblock_c = 0;
            while (*ptr != '`' && *ptr != '\0' && codeblock_c < 1024 - 1) {
                codeblock_buffer[codeblock_c] = *ptr;
                ptr++;
                codeblock_c++;
            }
            codeblock_buffer[codeblock_c] = '\0';
            tokens[*quantidade_tokens].tipo = CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, codeblock_buffer);
            (*quantidade_tokens)++;
        } else {
            /* Texto regular: coleta no buffer */
            if (indice_buffer < 1024 - 1) {
                buffer[indice_buffer++] = *ptr;
            }
        }
        ptr++;
    }

    /* Processa o texto restante */
    if (indice_buffer > 0) {
        buffer[indice_buffer] = '\0'; /* Finaliza o texto */
        tokens[*quantidade_tokens].tipo = TEXTO;
        strcpy(tokens[*quantidade_tokens].conteudo, buffer);
        (*quantidade_tokens)++;
    }
}

/**
 * @name renderizar_token
 * @return void
 * @param token:token_markdown_t - O token markdown que será renderizado.
 *
 * Esta função renderiza o token de markdown de acordo com o tipo de formatação
 * (cabeçalho, itálico, negrito, etc.), realizando modificações no texto (como
 * substituição de quebras de linha) e exibindo-o de forma apropriada no
 * terminal.
 **/
void renderizar_token(token_markdown_t token) {
    /* Declaração de variáveis */
    char *texto_modificado;

    /* Switch para processar o tipo de token */
    switch (token.tipo) {
    case CABECALHO:
        /* Substitui as quebras de linha e imprime o cabeçalho */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case ITALICO:
        /* Substitui as quebras de linha e imprime o texto em itálico */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case NEGRITO:
        /* Substitui as quebras de linha e imprime o texto em negrito */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case CODIGO:
        /* Substitui as quebras de linha e imprime o código */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case BLOCO_CITACAO:
        /* Substitui as quebras de linha e imprime o bloco de citação */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case LISTA:
        /* Indenta a lista de acordo com o nível de profundidade */
        for (int j = 0; j < token.nivel_lista; j++) {
            printf("\t");
        }
        /* Substitui as quebras de linha e imprime o item da lista */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    case BLOCO_CODIGO:
        /* Renderiza bloco de código com cores, se for linguagem C */
        if (strcmp(token.language, "c") == 0) {
            colorizar_codigo_em_c(token.conteudo);
        } else {
            printf("\033[37m%s\033[0m", token.conteudo);
        }
        break;
    case TEXTO:
        /* Substitui as quebras de linha e imprime o texto normal */
        texto_modificado = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", texto_modificado);
        break;
    }
}

/**
 * @name renderizar_tokens
 * @return void
 * @param tokens:token_markdown_t[] - O array de tokens markdown que será
 *renderizado.
 * @param quantidade_tokens: int - A quantidade total de tokens no array.
 * Descrição: Esta função percorre o array de tokens markdown e chama a função
 *`renderizar_token` para renderizar cada token individualmente.
 **/
void renderizar_tokens(token_markdown_t tokens[], int quantidade_tokens) {
    /* Declaração de variáveis */
    int i;

    /* Laço para iterar sobre todos os tokens e renderizá-los */
    for (i = 0; i < quantidade_tokens; i++) {
        renderizar_token(tokens[i]);
    }
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
    char **texto_processado;
    int initial_line_count;
    token_markdown_t tokens[128];

    altura_da_tela = 24;
    largura_da_tela = 80;
    linha_atual = 0;
    sair = 1;

    int quantidade_tokens = 0;
    int espacos_lista = 0;
    texto_processado = dividir_string_em_array(texto, &initial_line_count);

    for (int i = 0; i < initial_line_count; i++) {
        tokenizar(texto_processado[i], tokens, &quantidade_tokens,
                  &espacos_lista);
    }

    liberar_strings_dinamicas(texto_processado, initial_line_count);

    int numero_linhas;
    char **linhas = renderizar_tokens_para_strings(tokens, quantidade_tokens,
                                                   &numero_linhas);

    char **output = dividir_linhas(linhas, numero_linhas, &contagem_de_linhas);

    liberar_strings_dinamicas(linhas, numero_linhas);

    while (sair == 1) {
        limpar_tela();

        printf("+--------------------------------------------------------------"
               "----------------+\n");
        printf("|                             EMENTÁRIO INTERATIVO             "
               "                |\n");
        printf("+--------------------------------------------------------------"
               "----------------+\n");
        printf("\n");

        total_de_linhas = linha_atual + altura_da_tela;
        if (total_de_linhas > contagem_de_linhas) {
            total_de_linhas = contagem_de_linhas;
        }

        for (i = linha_atual; i < total_de_linhas; i++) {
            processar_string_cli(output[i]);
        }

        printf("\n");
        printf("+--------------------------------------------------------------"
               "----------------+\n");
        printf("|        %s        |                                       "
               "%04d/%04d LINHAS  |\n",
               (total_de_linhas == contagem_de_linhas) ? "FIM " : "MAIS",
               total_de_linhas, contagem_de_linhas);
        printf("+--------------------------------------------------------------"
               "----------------+\n");
        printf("|   Use [d] para descer    |    Use [s] para subir    |    use "
               "[q] prara sair  |\n");
        printf("+--------------------------------------------------------------"
               "----------------+\n");
        printf("\n");

        opcao = getchar();

        if (opcao == 'q') {
            sair = 0;
            break;
        } else if (opcao == 's') {
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

    liberar_strings_dinamicas(output, contagem_de_linhas);
}
