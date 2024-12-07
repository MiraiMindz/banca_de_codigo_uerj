#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_MAX_LINHA 80
#define TAM_MAX_ENTRADA 1024

#define COR_RESET "\033[0m"
#define COR_PALAVRA_CHAVE "\033[1;34m"   /* Azul Brilhante */
#define COR_COMENTARIO "\033[1;36m"   /* Ciano Brilhante */
#define COR_STRING "\033[1;32m"    /* Verde Brilhante */
#define COR_CARACTERE "\033[1;36m"      /* Ciano Brilhante */
#define COR_PREPROCESSADOR "\033[1;31m" /* Vermelho Brilhante */
#define COR_NUMERO "\033[1;35m"    /* Magenta Brilhante */
#define COR_OPERADOR "\033[1;33m" /* Amarelo Brilhante */
#define COR_INCLUDE_ANGULAR "\033[1;36m" /* Ciano Brilhante para <...> */
#define COR_INCLUDE_ANTIVIRGULA "\033[1;35m" /* Magenta Brilhante para "..." */

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

typedef enum {
    TEXTO,
    CABECALHO,
    ITALICO,
    NEGRITO,
    CODIGO,
    BLOCO_CITACAO,
    BLOCO_CODIGO,
    LISTA
} TipoToken;

typedef struct {
    TipoToken tipo;
    char conteudo[TAM_MAX_ENTRADA];
    int nivel_lista; // Store the nesting level of the list item
    char language[TAM_MAX_ENTRADA];
} Token;

/* Lista de palavras-chave do C89 */
const char *palavras_chave[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", 
    "double", "else", "enum", "extern", "float", "for", "goto", "if", 
    "int", "long", "register", "return", "short", "signed", "sizeof", 
    "static", "struct", "switch", "typedef", "union", "unsigned", "void", 
    "volatile", "while", NULL
};

const char *operadores[] = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", 
    "||", "!", "&", "|", "^", "~", "<<", ">>", "++", "--", "->", ".", 
    "?", ":", NULL
};



void tokenizar(char *linha, Token tokens[], int *quantidade_tokens, int *espacos_lista);
void renderizar_tokens(Token tokens[], int quantidade_tokens);
void quebrar_linha(char *texto);
void colorizar_codigo_em_c(const char *codigo);
int iniciar_com_operador(char c);
const char* encontrar_operador(const char *ptr);
int encontrar_palavra_chave(const char *palavra);

int main(void) {
    // Input lines (simulating user input)
    char entrada[][TAM_MAX_ENTRADA] = {
        "# cabeçalho\n",
        "**negrito**\n",
        "*italico*\n",
        "`inline code`\n",
        "Some **bo\n\nld** text and *italic* text and `inline code`.\n",
        "- a\n",
        "- b\n",
        "  - c\n",
        "  - d\n",
        "  - e\n",
        "  - e\n",
        "  - g\n",
        "> quote\n",
        "> quo\n",
        "te\n",
        "> quote\n",
        "> quote\n",
        "> quote\n",
        "```c\n",
        "#include <stdio.h>\n",
        "\n",
        "/* comentario */\n",
        "int main(void) {\n",
        "\tprintf(\"Hello World\n",
        "\t\taaa\");\n"
        "}\n",
        "```\n",
        "\n"};

    Token tokens[128];
    int quantidade_tokens = 0;
    int espacos_lista = 0; // Track the number of spaces for list indentation

    // Process input line-by-line
    for (int i = 0; i < sizeof(entrada) / sizeof(entrada[0]); i++) {
        tokenizar(entrada[i], tokens, &quantidade_tokens, &espacos_lista);
    }

    // Render tokens
    renderizar_tokens(tokens, quantidade_tokens);

    return 0;
}


/* Verifica se um caractere inicia um operador */
int iniciar_com_operador(char c) {
    return strchr("+-*/%=&|^~<>!?:.", c) != NULL;
}

/* Encontra o operador mais longo a partir da posição atual */
const char* encontrar_operador(const char *ptr) {
    int i;
    for (i = 0; operadores[i] != NULL; i++) {
        unsigned long len = strlen(operadores[i]);
        if (strncmp(ptr, operadores[i], len) == 0) {
            return operadores[i];
        }
    }
    return NULL;
}

/* Função auxiliar para verificar se uma palavra é uma palavra-chave do C89 */
int encontrar_palavra_chave(const char *palavra) {
    int i;
    for (i = 0; palavras_chave[i] != NULL; i++) {
        if (strcmp(palavra, palavras_chave[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Função para destacar a sintaxe do C89 */
void colorizar_codigo_em_c(const char *codigo) {
    const char *ptr = codigo;
    char buffer[256];
    int i;
    int colunas;
    colunas = 0;

    while (*ptr != '\0') {
        /* Lida com comentários */
        if (*ptr == '/' && *(ptr + 1) == '*') {
            printf(COR_COMENTARIO "/*");
            ptr += 2;
            colunas += 2;
            while (*ptr != '\0' && !(*ptr == '*' && *(ptr + 1) == '/')) {
                colunas++;
                if (colunas == 80) {
                    putchar('\n');
                    colunas = 0;
                }
                putchar(*ptr++);
            }
            if (*ptr == '*' && *(ptr + 1) == '/') {
                printf("*/" COR_RESET);
                ptr += 2;
            }
            colunas = 0;
            continue;
        }

        /* Lida com diretivas do pré-processador */
        if (*ptr == '#' && (ptr == codigo || *(ptr - 1) == '\n')) {
            if (*ptr == '#' && strncmp(ptr, "#include", 8) == 0 && isspace(*(ptr + 8))) {
                printf(COR_PREPROCESSADOR "#include");
                ptr += 8;

                /* Ignora espaços em branco */
                while (isspace(*ptr)) {
                    putchar(*ptr++);
                }
                /* Lida com <...> */
                if (*ptr == '<') {
                    printf(COR_INCLUDE_ANGULAR "<");
                    ptr++;
                    while (*ptr != '\0' && *ptr != '>') {
                        putchar(*ptr++);
                    }
                    if (*ptr == '>') {
                        printf(">" COR_RESET);
                        ptr++;
                    }
                    continue;
                }

                /* Lida com "..." */
                if (*ptr == '"') {
                    printf(COR_INCLUDE_ANTIVIRGULA "\"");
                    ptr++;
                    while (*ptr != '\0' && *ptr != '"') {
                        putchar(*ptr++);
                    }
                    if (*ptr == '"') {
                        printf("\"" COR_RESET);
                        ptr++;
                    }
                    continue;
                }

            } else {
                printf(COR_PREPROCESSADOR "#");
                ptr++;
                while (*ptr != '\0' && *ptr != '\n' && !(*ptr == '/' && *(ptr + 1) == '*')) {
                    putchar(*ptr++);
                }
                printf(COR_RESET);
                continue;
            }
        }

        /* Lida com caracteres entre aspas simples */
        if (*ptr == '\'') {
            printf(COR_CARACTERE "'");
            ptr++;
            if (*ptr == '\\') { /* Lida com caractere escapado */
                putchar(*ptr++);
                if (*ptr != '\0') {
                    putchar(*ptr++);
                }
            } else if (*ptr != '\0') {
                putchar(*ptr++);
            }
            if (*ptr == '\'') {
                printf("'" COR_RESET);
                ptr++;
            }
            continue;
        }

        /* Lida com strings entre aspas duplas */
        if (*ptr == '"') {
            printf(COR_STRING "\"");
            ptr++;
            while (*ptr != '\0' && *ptr != '"') {
                if (*ptr == '\\' && *(ptr + 1) == '"') { /* Lida com aspas duplas escapadas */
                    printf("\\\"");
                    ptr += 2;
                } else {
                    putchar(*ptr++);
                }
            }
            if (*ptr == '"') {
                printf("\"" COR_RESET);
                ptr++;
            }
            continue;
        }

        /* Lida com números */
        if (isdigit(*ptr) || (*ptr == '.' && isdigit(*(ptr + 1)))) {
            printf(COR_NUMERO);
            while (isdigit(*ptr) || *ptr == '.') {
                putchar(*ptr++);
            }
            printf(COR_RESET);
            continue;
        }

         /* Lida com operadores */
        if (iniciar_com_operador(*ptr)) {
            const char *op = encontrar_operador(ptr);
            if (op != NULL) {
                printf(COR_OPERADOR "%s" COR_RESET, op);
                ptr += strlen(op);
                continue;
            }
        }

        /* Lida com palavras-chave e identificadores */
        if (isalpha(*ptr) || *ptr == '_') {
            i = 0;
            while (isalnum(*ptr) || *ptr == '_') {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
            if (encontrar_palavra_chave(buffer)) {
                printf(COR_PALAVRA_CHAVE "%s" COR_RESET, buffer);
            } else {
                printf("%s", buffer);
            }
            continue;
        }

        /* Imprime outros caracteres */
        putchar(*ptr++);
    }
}


void tokenizar(char *linha, Token tokens[], int *quantidade_tokens, int *espacos_lista) {
    static int em_bloco_codigo = 0;
    char buffer[TAM_MAX_ENTRADA];
    int indice_buffer = 0;
    char *ptr = linha;

    // Check for code block start or end
    if (strncmp(linha, "```", 3) == 0) {
        if (em_bloco_codigo) {
            // Close code block
            tokens[*quantidade_tokens].tipo = BLOCO_CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, "");
            (*quantidade_tokens)++;
            em_bloco_codigo = 0;
        } else {
            // Start code block
            tokens[*quantidade_tokens].tipo = BLOCO_CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, "");
            // Check if there is a language identifier after the backticks
            char *language = linha + 3; // Skip the "```"
            while (*language == ' ' || *language == '\t') { // Skip spaces or tabs
                language++;
            }
            // Extract language (until we hit space, tab, or newline)
            if (*language != '\0' && *language != '\n') {
                char language_buffer[TAM_MAX_ENTRADA];
                int language_c = 0;
                while (*language != ' ' && *language != '\t' && *language != '\n' && *language != '\0') {
                    language_buffer[language_c++] = *language++;
                }
                language_buffer[language_c] = '\0';
                // Store language in a new token (LINGUAGEM_CODIGO type)
                strcpy(tokens[*quantidade_tokens].language, language_buffer);
            }
            
            (*quantidade_tokens)++;
            em_bloco_codigo = 1;
        }
        return;
    }

    // Handle code block content
    if (em_bloco_codigo) {
        // Collect content within the code block
        strcat(tokens[*quantidade_tokens - 1].conteudo, linha);
        return;
    }

    // Handle other types of tokens
    if (*linha == '>' && *(linha + 1) == ' ') {
        tokens[*quantidade_tokens].tipo = BLOCO_CITACAO;
        strcpy(tokens[*quantidade_tokens].conteudo, linha + 2);
        (*quantidade_tokens)++;
        return;
    }

    // Handle list items and track indentation level
    if (((*linha == '-' || *linha == '*') && *(linha + 1) == ' ') || (*linha == ' ') || (*linha == '\t')) {
        int identacao_lista = 0;
        int identacao_atual = 0;

        // Find indentation level for this line
        char *ch = linha;
        while ((*ch == ' ') || (*ch == '\t')) {
            identacao_lista++;
            ch++;
        }

        // Track indentation spaces for the first list item
        if (*espacos_lista == 0 && identacao_lista > 0) {
            *espacos_lista = identacao_lista; // Set the indentation for list items
        }

        // Avoid division by zero, check if the spaces for list indentation have been set
        if (*espacos_lista > 0 && identacao_lista % *espacos_lista == 0) {
            int nivel_lista = identacao_lista / *espacos_lista;
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = nivel_lista;
            strcpy(tokens[*quantidade_tokens].conteudo, ch); // Save list item content
            (*quantidade_tokens)++;
        } else if (*espacos_lista == 0) {
            // no indentation
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = 0;
            strcpy(tokens[*quantidade_tokens].conteudo, ch); // Save list item content
            (*quantidade_tokens)++;
        }

        return;
    }

    // Process inline formatting (Bold, Italics, etc.)
    char* start_idx;
    while (*ptr) {
        if (*ptr == '#' && (ptr == linha || *(ptr - 1) == ' ')) {
            char title_buffer[TAM_MAX_ENTRADA];
            int title_c = 0;
            ptr += 2;
            while ((*ptr != '\n') && (title_c < TAM_MAX_ENTRADA - 1)) {
                title_buffer[title_c] = *ptr;
                ptr++;
                title_c++;
            }
            if (*ptr == '\n' && title_c < TAM_MAX_ENTRADA - 1) {
                title_buffer[title_c] = '\n'; 
                title_c++;
            }

            title_buffer[title_c] = '\0';
            tokens[*quantidade_tokens].tipo = CABECALHO;
            strcpy(tokens[*quantidade_tokens].conteudo, title_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '*' && *(ptr + 1) == '*' && indice_buffer < TAM_MAX_ENTRADA - 1) {
            // If there's regular text before the bold formatting, process it
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0';  // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0;  // Reset the buffer for new content
            }

            // Handle bold formatting: collect bold text
            ptr += 2;  // Skip the '**'
            char bold_buffer[TAM_MAX_ENTRADA];
            int bold_c = 0;
            while (!(*ptr == '*' && *(ptr + 1) == '*') && bold_c < TAM_MAX_ENTRADA - 1) {
                bold_buffer[bold_c] = *ptr;
                ptr++;
                bold_c++;
            }
            ptr++;  // Skip the second '*' character
            bold_buffer[bold_c] = '\0';
            tokens[*quantidade_tokens].tipo = NEGRITO;
            strcpy(tokens[*quantidade_tokens].conteudo, bold_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '*' || *ptr == '_') {
            // Italics formatting: collect italic text
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0';  // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0;  // Reset the buffer for new content
            }

            ptr++;  // Skip the '*' or '_'
            char italic_buffer[TAM_MAX_ENTRADA];
            int italic_c = 0;
            while (!(*ptr == '*' || *ptr == '_') && italic_c < TAM_MAX_ENTRADA - 1) {
                italic_buffer[italic_c] = *ptr;
                ptr++;
                italic_c++;
            }
            italic_buffer[italic_c] = '\0';
            tokens[*quantidade_tokens].tipo = ITALICO;
            strcpy(tokens[*quantidade_tokens].conteudo, italic_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '`') {
            // Code formatting: collect inline code
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0';  // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0;  // Reset the buffer for new content
            }

            ptr++;  // Skip the '`'
            char codeblock_buffer[TAM_MAX_ENTRADA];
            int codeblock_c = 0;
            while (*ptr != '`' && *ptr != '\0' && codeblock_c < TAM_MAX_ENTRADA - 1) {
                codeblock_buffer[codeblock_c] = *ptr;
                ptr++;
                codeblock_c++;
            }
            codeblock_buffer[codeblock_c] = '\0';
            tokens[*quantidade_tokens].tipo = CODIGO;
            strcpy(tokens[*quantidade_tokens].conteudo, codeblock_buffer);
            (*quantidade_tokens)++;
        } else {
            // Regular text: collect in the buffer
            if (indice_buffer < TAM_MAX_ENTRADA - 1) {
                buffer[indice_buffer++] = *ptr;
            }
        }
        ptr++;
    }

    // Process any remaining text
    if (indice_buffer > 0) {
        buffer[indice_buffer] = '\0';  // Null-terminate the text
        tokens[*quantidade_tokens].tipo = TEXTO;
        strcpy(tokens[*quantidade_tokens].conteudo, buffer);
        (*quantidade_tokens)++;
    }
}

void renderizar_tokens(Token tokens[], int quantidade_tokens) {
    for (int i = 0; i < quantidade_tokens; i++) {
        // Render based on token type
        switch (tokens[i].tipo) {
        case CABECALHO:
            printf("\033[31m");
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case ITALICO:
            printf("\033[32m");
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case NEGRITO:
            printf("\033[33m");
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case CODIGO:
            printf("\033[34m");
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case BLOCO_CITACAO:
            printf("\033[36m> ");
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case LISTA:
            // Indent based on list level
            printf("\033[35m");
            for (int j = 0; j < tokens[i].nivel_lista; j++) {
                printf("\t");
            }
            quebrar_linha(tokens[i].conteudo);
            printf("\033[0m");
            break;
        case BLOCO_CODIGO:
            // Render block code with proper coloring
            if (strcmp(tokens[i].language, "c") == 0) {
                colorizar_codigo_em_c(tokens[i].conteudo);
            } else {
                printf("\033[37m%s\033[0m", tokens[i].conteudo);
            }
            break;
        case TEXTO:
            quebrar_linha(tokens[i].conteudo);
            break;
        }
    }
}



void quebrar_linha(char *texto) {
    int comprimento_linha = 0;
    char *ptr = texto;

    while (*ptr) {
        if (comprimento_linha >= TAM_MAX_LINHA && *ptr == ' ') {
            printf("\n");
            comprimento_linha = 0;
        } else {
            putchar(*ptr);
            comprimento_linha++;
        }
        ptr++;
    }
    // printf("\n");
}
