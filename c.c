#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

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
void renderizar_token(Token tokens);
void colorizar_codigo_em_c(const char *codigo);
int iniciar_com_operador(char c);
const char* encontrar_operador(const char *ptr);
int encontrar_palavra_chave(const char *palavra);
char** colorizar_codigo_em_c_str_arr(char *codigo, unsigned long *num_linhas);
char *duplicar_string(const char *origem);

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


/* Função para duplicar uma string (substituindo strdup) */
char *duplicar_string(const char *origem) {
    unsigned long tamanho;
    char *nova_string;

    tamanho = strlen(origem) + 1; /* Inclui o caractere nulo */
    nova_string = malloc(tamanho);
    if (nova_string == NULL) {
        fprintf(stderr, "Erro de alocação de memória na duplicação de string\n");
        exit(1);
    }
    strcpy(nova_string, origem);
    return nova_string;
}

/* Function to check if a character is the start of an ANSI escape sequence */
int is_escape_sequence_start(const char *str, int i) {
    return str[i] == '\033' && str[i + 1] == '[';
}

/* Function to calculate the length of an ANSI escape sequence */
int escape_sequence_length(const char *str, int start) {
    int len = 0;
    if (is_escape_sequence_start(str, start)) {
        len = 2; // '\033['
        start += 2;
        while (str[start] && !isalpha(str[start])) {
            len++;
            start++;
        }
        len++; // The final letter of the escape sequence
    }
    return len;
}

/* Split a single line into chunks */
void split_line(const char *line, char ***result, int *result_count) {
    int len = strlen(line);
    int visible_count = 0, start = 0, last_space = -1;
    int i = 0;
    *result_count = 0;
    *result = NULL;

    while (i < len) {
        if (is_escape_sequence_start(line, i)) {
            int esc_len = escape_sequence_length(line, i);
            i += esc_len; // Skip the escape sequence
        } else if (line[i] == ' ') {
            last_space = i;
            visible_count++;
            i++;
        } else {
            visible_count++;
            i++;
        }

        if (visible_count > 80) {
            int split_at = (last_space != -1) ? last_space : i - 1;
            int chunk_length = split_at - start + 1;
            char *chunk = (char *)malloc(chunk_length + 2); // +2 for \n and \0
            strncpy(chunk, line + start, chunk_length);
            chunk[chunk_length] = '\0';

            // Append \n if not already present
            if (chunk[chunk_length - 1] != '\n') {
                strcat(chunk, "\n");
            }

            *result = (char **)realloc(*result, (*result_count + 1) * sizeof(char *));
            (*result)[*result_count] = chunk;
            (*result_count)++;

            start = split_at + 1;
            visible_count = i - start;
            last_space = -1;
        }
    }

    if (start < len) {
        int chunk_length = len - start;
        char *chunk = (char *)malloc(chunk_length + 2); // +2 for \n and \0
        strncpy(chunk, line + start, chunk_length);
        chunk[chunk_length] = '\0';

        *result = (char **)realloc(*result, (*result_count + 1) * sizeof(char *));
        (*result)[*result_count] = chunk;
        (*result_count)++;
    }
}

/* Split an array of strings into chunks */
char **split_lines(char **input, int input_count, int *output_count) {
    char **result = NULL;
    int result_size = 0;

    for (int i = 0; i < input_count; i++) {
        char **split_result = NULL;
        int split_count = 0;
        split_line(input[i], &split_result, &split_count);

        result = (char **)realloc(result, (result_size + split_count) * sizeof(char *));
        for (int j = 0; j < split_count; j++) {
            result[result_size++] = split_result[j];
        }
        free(split_result);
    }

    *output_count = result_size;
    return result;
}

/* Function to copy a static string array to a dynamically allocated string array */
char **copy_static_to_dynamic(char *static_strings[], int count) {
    // Allocate memory for the array of pointers
    char **dynamic_strings = (char **)calloc(count, sizeof(char *));
    if (!dynamic_strings) {
        fprintf(stderr, "Error allocating memory for dynamic array.\n");
        exit(EXIT_FAILURE);
    }

    // Copy each string from the static array to the dynamic array
    for (int i = 0; i < count; i++) {
        dynamic_strings[i] = duplicar_string(static_strings[i]);
        if (!dynamic_strings[i]) {
            fprintf(stderr, "Error allocating memory for string copy.\n");
            // Free previously allocated strings
            for (int j = 0; j < i; j++) {
                free(dynamic_strings[j]);
            }
            free(dynamic_strings);
            exit(EXIT_FAILURE);
        }
    }

    return dynamic_strings;
}

/* Free a dynamically allocated array of strings */
void free_dynamic_strings(char **strings, int count) {
    for (int i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
}



char* replace_newlines(const char* input) {
    // Calculate the size of the new string, worst case is double the original length
    int new_size = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        new_size++;
        if (input[i] == '\n') {
            new_size++; // for the extra backslash
        }
    }

    // Allocate memory for the new string
    char* result = (char*)malloc(new_size + 1); // +1 for null terminator
    if (!result) {
        return NULL; // Memory allocation failed
    }

    int j = 0; // Index for the result string
    for (int i = 0; input[i] != '\0'; ++i) {
        if (input[i] == '\n') {
            result[j++] = '\\';  // Insert backslash
            result[j++] = 'n';   // Insert 'n'
        } else {
            result[j++] = input[i]; // Copy the character as is
        }
    }

    result[j] = '\0'; // Null-terminate the new string
    return result;
}

void paginador(char **texto, int contagem_de_linhas) {
    int altura_da_tela;
    int largura_da_tela;
    char opcao;
    char **texto_divido;
    int i;
    int total_de_linhas;
    int linha_atual;
    int sair;

    altura_da_tela = 24;
    largura_da_tela = 80;
    linha_atual = 0;
    sair = 1;

    while (sair == 1) {
        limpar_tela();
        total_de_linhas = linha_atual + altura_da_tela;
        if (total_de_linhas > contagem_de_linhas) {
            total_de_linhas = contagem_de_linhas;
        }

        for (i = linha_atual; i < total_de_linhas; i++) {
            printf("%s", texto[i]);
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
}

char *printf_para_string(const char *formato, ...) {
    va_list args;
    int size;
    char temp_buffer[1024];  // Temporary buffer for estimating size

    // Initialize the va_list for variable arguments
    va_start(args, formato);

    // First pass: Format the string into a temporary buffer to estimate size
    size = vsprintf(temp_buffer, formato, args);

    // If size is negative, there was an error in formatting
    if (size < 0) {
        va_end(args);
        return NULL;
    }

    // Allocate memory for the final buffer (size + 1 for null terminator)
    char *buffer = (char *)malloc(size + 1);
    if (buffer == NULL) {
        // Handle memory allocation failure
        va_end(args);
        return NULL;
    }

    // Reinitialize va_list before second pass
    va_start(args, formato);

    // Second pass: Format the string into the allocated buffer
    vsprintf(buffer, formato, args);

    // Clean up the va_list
    va_end(args);

    return buffer;
}



char **renderizar_tokens_para_strings(Token tokens[], int quantidade_tokens, int *numero_linhas) {
    int tamanho;
    char *linha;
    char* token_atual;
    char **linhas_codigo;
    unsigned long num_linhas_codigo;
    int linhas_alocadas;
    linha = calloc(512, sizeof(char)); // Aloca memória para uma linha inicial
    char **linhas = calloc(quantidade_tokens, sizeof(char *)); // Aloca espaço para armazenar as linhas
    if (linhas == NULL) {
        return NULL;
    }

    *numero_linhas = 0; // Inicializa o contador de linhas

    char *linha_atual = calloc(512, sizeof(char));
    if (linha_atual == NULL) {
        // Se falhar a alocação, libera o que foi alocado e retorna NULL
        for (int j = 0; j < *numero_linhas; j++) {
            free(linhas[j]);
        }
        free(linhas);
        return NULL;
    }

    token_atual = calloc(512, sizeof(char));
    if (token_atual == NULL) {
        // Se falhar a alocação, libera o que foi alocado e retorna NULL
        for (int j = 0; j < *numero_linhas; j++) {
            free(linhas[j]);
        }
        free(linhas);
        return NULL;
    }

    for (int i = 0; i < quantidade_tokens; i++) {
        // Dependendo do tipo do token, formatamos a linha
        switch (tokens[i].tipo) {
            case CABECALHO:
                token_atual = printf_para_string("\033[31m%s\033[0m", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
            case ITALICO:
                token_atual = printf_para_string("\033[32m%s\033[0m", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
            case NEGRITO:
                token_atual = printf_para_string("\033[33m%s\033[0m", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
            case CODIGO:
                token_atual = printf_para_string("\033[34m%s\033[0m", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
            case BLOCO_CITACAO:
                token_atual = printf_para_string("\033[36m> %s\033[0m", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
            case LISTA:
                token_atual = printf_para_string("\033[35m%s\033[0m", tokens[i].conteudo);
                for (int j = 0; j < tokens[i].nivel_lista; j++) {
                    token_atual = realloc(token_atual, strlen(token_atual) + 2);
                    if (token_atual == NULL) {
                        return NULL;
                    }
                    memmove(token_atual + 1, token_atual, strlen(token_atual) + 1);
                    token_atual[0] = '\t';
                }
                strcat(linha_atual, token_atual);
                break;
            case BLOCO_CODIGO:
                if (strcmp(tokens[i].language, "c") == 0) {
                    linhas_codigo = colorizar_codigo_em_c_str_arr(tokens[i].conteudo, &num_linhas_codigo);
                    if (linhas_codigo == NULL) {
                        for (int j = 0; j < *numero_linhas; j++) {
                            free(linhas[j]);
                        }
                        free(linhas);
                        return NULL;
                    }
                    for (unsigned long j = 0; j < num_linhas_codigo; j++) {
                        linhas[*numero_linhas] = duplicar_string(linhas_codigo[j]);
                        if (linhas[*numero_linhas] == NULL) {
                            for (int k = 0; k < *numero_linhas; k++) {
                                free(linhas[k]);
                            }
                            for (unsigned long k = 0; k < num_linhas_codigo; k++) {
                                free(linhas_codigo[k]);
                            }
                            free(linhas_codigo);
                            free(linhas);
                            return NULL;
                        }
                        (*numero_linhas)++;
                    }
                    for (unsigned long j = 0; j < num_linhas_codigo; j++) {
                        free(linhas_codigo[j]);
                    }
                    free(linhas_codigo);
                    continue; 
                } else {
                    token_atual = printf_para_string("\033[37m%s\033[0m", tokens[i].conteudo);
                    strcat(linha_atual, token_atual);
                }
                break;
            case TEXTO:
                token_atual = printf_para_string("%s", tokens[i].conteudo);
                strcat(linha_atual, token_atual);
                break;
        }

         if (token_atual == NULL) {
            for (int j = 0; j < *numero_linhas; j++) {
                free(linhas[j]);
            }
            free(linhas);
            return NULL;
        }

        if (linha_atual == NULL) {
            for (int j = 0; j < *numero_linhas; j++) {
                free(linhas[j]);
            }
            free(linhas);
            return NULL;
        }

        if (strchr(linha_atual, '\n') != NULL) {
            linhas[*numero_linhas] = duplicar_string(linha_atual);
            (*numero_linhas)++;
            memset(linha_atual, '\0', 512);
            continue;
        }
    }

    return linhas;
}

int main(void) {
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

    char *entrada[] = {
        "# cabeçalho\n",
        "**negrito**\n",
        "*italico*\n",
        "`inline code` *italico* **negrito**\n",
        "**negrito** `inline code` *italico*\n",
        "*italico* **negrito** `inline code`\n",
        "**code without a newline** ",
        "*This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart*\n",
        "**This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart**\n",
        "`This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart`\n",
        "- a\n",
        "- b\n",
        "  - c\n",
        "  - d\n",
        "  - e\n",
        "    - e\n",
        "  - g\n",
        "> quote\n",
        "> quote\n",
        "> quote\n",
        "> quote\n",
        "```c\n",
        "#include <stdio.h>\n",
        "\n",
        "/* comentario */\n",
        "int main(void) {\n",
        "\tprintf(\"Hello World\\n\");\n",
        "\n",
        "\treturn 0;\n",
        "}\n",
        "```\n",
        "\n"
        };
    int static_count = sizeof(entrada) / sizeof(entrada[0]);

    // Copy the static array to a dynamic array
    char **dynamic_strings = copy_static_to_dynamic(entrada, static_count);

    Token tokens[128];
    int quantidade_tokens = 0;
    int espacos_lista = 0; 

    for (int i = 0; i < static_count; i++) {
        tokenizar(dynamic_strings[i], tokens, &quantidade_tokens, &espacos_lista);
    }


    int numero_linhas;
    char **linhas = renderizar_tokens_para_strings(tokens, quantidade_tokens, &numero_linhas);



    int output_count = 0;
    /* Process the input array */
    char **output = split_lines(linhas, numero_linhas, &output_count);

    // paginador(linhas, numero_linhas);
    for (int i = 0; i < output_count; i++) {
        cli_processar_string(output[i]);
        free(output[i]);
    }
    free(output);

    free_dynamic_strings(linhas, numero_linhas);

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


char** colorizar_codigo_em_c_str_arr(char *codigo, unsigned long *num_linhas) {
    const char *ponteiro;
    char buffer[256];
    unsigned long capacidade_linhas;
    unsigned long tamanho_linha;
    unsigned long capacidade_linha;
    char **linhas;
    char *linha_atual;

    ponteiro = codigo;
    capacidade_linhas = 10;
    *num_linhas = 0;

    linhas = malloc(capacidade_linhas * sizeof(char*));
    linha_atual = malloc(256);
    tamanho_linha = 0;
    capacidade_linha = 256;

    if (!linhas || !linha_atual) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }

    linha_atual[0] = '\0';

    while (*ponteiro != '\0') {
        /* Lida com comentários */
        if (*ponteiro == '/' && *(ponteiro + 1) == '*') {
            strcat(linha_atual, COR_COMENTARIO "/*");
            ponteiro += 2;
            tamanho_linha += 4;
            while (*ponteiro != '\0' && !(*ponteiro == '*' && *(ponteiro + 1) == '/')) {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            if (*ponteiro == '*' && *(ponteiro + 1) == '/') {
                strcat(linha_atual, "*/" COR_RESET);
                ponteiro += 2;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com diretivas do pré-processador */
        if (*ponteiro == '#' && (ponteiro == codigo || *(ponteiro - 1) == '\n')) {
            if (strncmp(ponteiro, "#include", 8) == 0 && isspace(*(ponteiro + 8))) {
                strcat(linha_atual, COR_PREPROCESSADOR "#include");
                ponteiro += 8;

                /* Ignora espaços em branco */
                while (isspace(*ponteiro)) {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }

                /* Lida com <...> */
                if (*ponteiro == '<') {
                    strcat(linha_atual, COR_INCLUDE_ANGULAR "<");
                    ponteiro++;
                    tamanho_linha++;
                    while (*ponteiro != '\0' && *ponteiro != '>') {
                        strncat(linha_atual, ponteiro++, 1);
                        tamanho_linha++;
                    }
                    if (*ponteiro == '>') {
                        strncat(linha_atual, ponteiro++, 1);
                        strcat(linha_atual, COR_RESET);
                        tamanho_linha += 2;
                    }
                    continue;
                }

                /* Lida com "..." */
                if (*ponteiro == '"') {
                    strcat(linha_atual, COR_INCLUDE_ANTIVIRGULA "\"");
                    ponteiro++;
                    tamanho_linha++;
                    while (*ponteiro != '\0' && *ponteiro != '"') {
                        strncat(linha_atual, ponteiro++, 1);
                        tamanho_linha++;
                    }
                    if (*ponteiro == '"') {
                        strncat(linha_atual, ponteiro++, 1);
                        strcat(linha_atual, COR_RESET);
                        tamanho_linha++;
                    }
                    continue;
                }
            } else {
                strcat(linha_atual, COR_PREPROCESSADOR "#");
                ponteiro++;
                tamanho_linha++;
                while (*ponteiro != '\0' && *ponteiro != '\n' && !(*ponteiro == '/' && *(ponteiro + 1) == '*')) {
                    strncat(linha_atual, ponteiro++, 1);
                    tamanho_linha++;
                }
                strcat(linha_atual, COR_RESET);
                continue;
            }
        }

        /* Lida com caracteres entre aspas simples */
        if (*ponteiro == '\'') {
            strcat(linha_atual, COR_CARACTERE "'");
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
                strcat(linha_atual, "'" COR_RESET);
                ponteiro++;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com strings entre aspas duplas */
        if (*ponteiro == '"') {
            strcat(linha_atual, COR_STRING "\"");
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
                strcat(linha_atual, "\"" COR_RESET);
                ponteiro++;
                tamanho_linha += 5;
            }
            continue;
        }

        /* Lida com números */
        if (isdigit(*ponteiro) || (*ponteiro == '.' && isdigit(*(ponteiro + 1)))) {
            strcat(linha_atual, COR_NUMERO);
            tamanho_linha += strlen(COR_NUMERO);
            while (isdigit(*ponteiro) || *ponteiro == '.') {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            strcat(linha_atual, COR_RESET);
            tamanho_linha += strlen(COR_RESET);
            continue;
        }

        /* Lida com operadores */
        if (iniciar_com_operador(*ponteiro)) {
            const char *op;
            op = encontrar_operador(ponteiro);
            if (op != NULL) {
                strcat(linha_atual, COR_OPERADOR);
                strcat(linha_atual, op);
                strcat(linha_atual, COR_RESET);
                ponteiro += strlen(op);
                tamanho_linha += strlen(op) + strlen(COR_OPERADOR) + strlen(COR_RESET);
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
                strcat(linha_atual, COR_PALAVRA_CHAVE);
                strcat(linha_atual, buffer);
                strcat(linha_atual, COR_RESET);
                tamanho_linha += strlen(buffer) + strlen(COR_PALAVRA_CHAVE) + strlen(COR_RESET);
            } else {
                strcat(linha_atual, buffer);
                tamanho_linha += strlen(buffer);
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
                linhas = realloc(linhas, capacidade_linhas * sizeof(char*));
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
        renderizar_token(tokens[i]);
    }
}

void renderizar_token(Token token) {
    char* modified_text;
    switch (token.tipo) {
        case CABECALHO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s CABECALHO>\n", modified_text);
            break;
        case ITALICO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s ITALICO>\n", modified_text);
            break;
        case NEGRITO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s NEGRITO>\n", modified_text);
            break;
        case CODIGO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s CODIGO>\n", modified_text);
            break;
        case BLOCO_CITACAO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s BLOCO_CITACAO>\n", modified_text);
            break;
        case LISTA:
            // Indent based on list level
            for (int j = 0; j < token.nivel_lista; j++) {
                printf("\t");
            }
            modified_text = replace_newlines(token.conteudo);
            printf("<%s LISTA>\n", modified_text);
            break;
        case BLOCO_CODIGO:
            // Render block code with proper coloring
            if (strcmp(token.language, "c") == 0) {
                colorizar_codigo_em_c(token.conteudo);
            } else {
                printf("\033[37m%s\033[0m", token.conteudo);
            }
            break;
        case TEXTO:
            modified_text = replace_newlines(token.conteudo);
            printf("<%s TEXTO>\n", modified_text);
            break;
        }
}

char** renderizar_tokens_str(Token token) {
    char* linha;
    linha = NULL;
    switch (token.tipo) {
        case CABECALHO:
            printf("\033[31m");
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case ITALICO:
            printf("\033[32m");
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case NEGRITO:
            printf("\033[33m");
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case CODIGO:
            printf("\033[34m");
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case BLOCO_CITACAO:
            printf("\033[36m> ");
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case LISTA:
            // Indent based on list level
            printf("\033[35m");
            for (int j = 0; j < token.nivel_lista; j++) {
                printf("\t");
            }
            printf("%s", token.conteudo);
            printf("\033[0m");
            break;
        case BLOCO_CODIGO:
            // Render block code with proper coloring
            if (strcmp(token.language, "c") == 0) {
                colorizar_codigo_em_c(token.conteudo);
            } else {
                printf("\033[37m%s\033[0m", token.conteudo);
            }
            break;
        case TEXTO:
            printf("%s", token.conteudo);
            break;
        }
}
