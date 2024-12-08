#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

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

/* Função para obter a sequência de escape ANSI para uma cor, com foreground ou
 * background */
const char *obter_codigo_ansi(cores_terminal_t cor, int tipo) {
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
    } else if (tipo == 1) { // Background
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

void tokenizar(char *linha, token_markdown_t tokens[], int *quantidade_tokens,
               int *espacos_lista);
void renderizar_tokens(token_markdown_t tokens[], int quantidade_tokens);
void renderizar_token(token_markdown_t tokens);
char **colorizar_codigo_em_c_str_arr(char *codigo, unsigned long *num_linhas);
void colorizar_codigo_em_c(const char *codigo);
int iniciar_com_operador(char c);
const char *encontrar_operador(const char *ptr);
int encontrar_palavra_chave(const char *palavra);
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
                    } else if (value >= 30 &&
                               value <= 37) { // Foreground bright
                        fg_color = value - 30 + PRETO_ESCURO;
                    } else if (value >= 90 && value <= 97) { // Foreground light
                        fg_color = value - 90 + PRETO_CLARO;
                    } else if (value >= 40 &&
                               value <= 47) { // Background bright
                        bg_color = value - 40 + PRETO_ESCURO;
                    } else if (value >= 100 &&
                               value <= 107) { // Background light
                        bg_color = value - 100 + PRETO_CLARO;
                    }

                    if (*ptr == ';')
                        ptr++; // Skip separator
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

    tamanho = contar_caracteres_utf8(origem) + 1; /* Inclui o caractere nulo */
    nova_string = malloc(tamanho);
    if (nova_string == NULL) {
        fprintf(stderr,
                "Erro de alocação de memória na duplicação de string\n");
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
    int len = contar_caracteres_utf8(line);
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

            *result =
                (char **)realloc(*result, (*result_count + 1) * sizeof(char *));
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

        *result =
            (char **)realloc(*result, (*result_count + 1) * sizeof(char *));
        (*result)[*result_count] = chunk;
        (*result_count)++;
    }
}

/* Split an array of strings into chunks */
char **split_lines(char **input, int input_count, int *output_count) {
    char **result = NULL;
    size_t result_size = 0;

    for (int i = 0; i < input_count; i++) {
        char **split_result = NULL;
        int split_count = 0;
        split_line(input[i], &split_result, &split_count);

        result = (char **)realloc(result,
                                  (result_size + split_count) * sizeof(char *));
        for (int j = 0; j < split_count; j++) {
            result[result_size++] = split_result[j];
        }
        free(split_result);
    }

    *output_count = result_size;
    return result;
}

/* Function to copy a static string array to a dynamically allocated string
 * array */
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

/* Função para substituir todas as ocorrências de velho_sub por novo_sub em str.
 */
char *substituir_str(char *str, char *velho_sub, char *novo_sub) {
    /* Declarações */
    char *temp, *resultado, *atual, *destino;
    unsigned long comprimento_str, comprimento_velho_sub, comprimento_novo_sub;
    unsigned long contador, comprimento_segmento, comprimento_resultado;

    /* Validação de entradas. */
    if (str == NULL || velho_sub == NULL || novo_sub == NULL) {
        return NULL; /* Entrada inválida. */
    }

    if (*velho_sub == '\0') {
        return NULL; /* Substring vazia para substituir. */
    }

    /* Inicialização das variáveis. */
    comprimento_str = contar_caracteres_utf8(str);
    comprimento_velho_sub = contar_caracteres_utf8(velho_sub);
    comprimento_novo_sub = contar_caracteres_utf8(novo_sub);
    contador = 0;

    /* Contar ocorrências de velho_sub em str. */
    temp = str;
    while ((temp = strstr(temp, velho_sub)) != NULL) {
        contador++;
        temp += comprimento_velho_sub;
    }

    /* Calcular o comprimento da string resultante. */
    comprimento_resultado =
        comprimento_str +
        contador * (comprimento_novo_sub - comprimento_velho_sub);

    /* Alocar memória para a nova string. */
    resultado = (char *)malloc(comprimento_resultado +
                               1); /* +1 para o terminador nulo. */
    if (resultado == NULL) {
        return NULL; /* Falha na alocação de memória. */
    }

    /* Inicializar ponteiros para o processamento. */
    atual = str;
    destino = resultado;

    /* Substituir ocorrências de velho_sub por novo_sub. */
    while ((temp = strstr(atual, velho_sub)) != NULL) {
        /* Calcular o comprimento do segmento antes de velho_sub. */
        comprimento_segmento = (unsigned long)(temp - atual);

        /* Copiar o segmento antes de velho_sub. */
        memcpy(destino, atual, comprimento_segmento);
        destino += comprimento_segmento;

        /* Copiar novo_sub no lugar de velho_sub. */
        memcpy(destino, novo_sub, comprimento_novo_sub);
        destino += comprimento_novo_sub;

        /* Avançar além de velho_sub na string original. */
        atual = temp + comprimento_velho_sub;
    }

    /* Copiar o restante da string original. */
    strcpy(destino, atual);

    return resultado;
}

void renderizar_token_debug(token_markdown_t token) {
    char *modified_text;
    switch (token.tipo) {
    case CABECALHO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s CABECALHO>\n", modified_text);
        break;
    case ITALICO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s ITALICO>\n", modified_text);
        break;
    case NEGRITO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s NEGRITO>\n", modified_text);
        break;
    case CODIGO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s CODIGO>\n", modified_text);
        break;
    case BLOCO_CITACAO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s BLOCO_CITACAO>\n", modified_text);
        break;
    case LISTA:
        // Indent based on list level
        for (int j = 0; j < token.nivel_lista; j++) {
            printf("\t");
        }
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s LISTA>\n", modified_text);
        break;
    case BLOCO_CODIGO:
        printf("<%s BLOCO_CODIGO[%s]>", token.conteudo, token.language);
        break;
    case TEXTO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("<%s TEXTO>\n", modified_text);
        break;
    }
}

void renderizar_tokens_debug(token_markdown_t tokens[], int quantidade_tokens) {
    for (int i = 0; i < quantidade_tokens; i++) {
        renderizar_token_debug(tokens[i]);
    }
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
    char temp_buffer[1024]; // Temporary buffer for estimating size

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

char **renderizar_tokens_para_strings(token_markdown_t tokens[],
                                      int quantidade_tokens,
                                      int *numero_linhas) {
    int tamanho;
    char *linha;
    char *token_atual;
    char **linhas_codigo;
    unsigned long num_linhas_codigo;
    int linhas_alocadas;
    linha = calloc(512, sizeof(char)); // Aloca memória para uma linha inicial
    char **linhas =
        calloc(quantidade_tokens,
               sizeof(char *)); // Aloca espaço para armazenar as linhas
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
        switch (tokens[i].tipo) {
        case CABECALHO:
            token_atual = printf_para_string(
                "%s%s%s%s%s", obter_codigo_ansi(BRANCO_CLARO, 1),
                obter_codigo_ansi(PRETO_ESCURO, 0), tokens[i].conteudo,
                obter_codigo_ansi(PADRAO, 0), obter_codigo_ansi(PADRAO, 1));
            strcat(linha_atual, token_atual);
            break;
        case ITALICO:
            token_atual = printf_para_string(
                "%s%s%s", obter_codigo_ansi(AMARELO_ESCURO, 0),
                tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
            strcat(linha_atual, token_atual);
            break;
        case NEGRITO:
            token_atual = printf_para_string(
                "%s%s%s", obter_codigo_ansi(BRANCO_CLARO, 0),
                tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
            strcat(linha_atual, token_atual);
            break;
        case CODIGO:
            token_atual = printf_para_string(
                "%s%s%s", obter_codigo_ansi(AZUL_CLARO, 0), tokens[i].conteudo,
                obter_codigo_ansi(PADRAO, 0));
            strcat(linha_atual, token_atual);
            break;
        case BLOCO_CITACAO:
            token_atual = printf_para_string(
                "%s> %s%s", obter_codigo_ansi(PRETO_CLARO, 0),
                tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
            strcat(linha_atual, token_atual);
            break;
        case LISTA:
            token_atual = printf_para_string(
                "%s%s%s", obter_codigo_ansi(BRANCO_ESCURO, 0),
                tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
            for (int j = 0; j < tokens[i].nivel_lista; j++) {
                token_atual = realloc(token_atual, contar_caracteres_utf8(token_atual) + 2);
                if (token_atual == NULL) {
                    return NULL;
                }
                memmove(token_atual + 1, token_atual, contar_caracteres_utf8(token_atual) + 1);
                token_atual[0] = '\t';
            }
            strcat(linha_atual, token_atual);
            break;
        case BLOCO_CODIGO:
            if (strcmp(tokens[i].language, "c") == 0) {
                linhas_codigo = colorizar_codigo_em_c_str_arr(
                    tokens[i].conteudo, &num_linhas_codigo);
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
                token_atual = printf_para_string(
                    "%s%s%s", obter_codigo_ansi(AZUL_ESCURO, 0),
                    tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
                strcat(linha_atual, token_atual);
            }
            break;
        case TEXTO:
            token_atual = printf_para_string(
                "%s%s%s", obter_codigo_ansi(BRANCO_ESCURO, 0),
                tokens[i].conteudo, obter_codigo_ansi(PADRAO, 0));
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

char **split_string_block_into_array(char *input, int *count) {
    char **result = NULL;      /* Array of strings */
    int capacity = 0;          /* Current capacity of the array */
    int line_count = 0;        /* Number of lines found */
    const char *start = input; /* Pointer to the start of a substring */
    result = calloc(512, sizeof(char *));

    while (*input != '\0') {
        if (*input == '\n') {
            /* Allocate space for the array if needed */
            if (line_count >= capacity) {
                capacity = (capacity == 0) ? 1 : capacity * 2;
                result = (char **)realloc(result, capacity * sizeof(char *));
                if (result == NULL) {
                    perror("Failed to realloc memory");
                    // free(result);
                    exit(EXIT_FAILURE);
                }
            }

            /* Allocate and copy the current line including the newline */
            int length = input - start;
            result[line_count] = (char *)malloc(
                (length + 2) * sizeof(char)); /* +2 for '\n' and '\0' */
            if (result[line_count] == NULL) {
                perror("Failed to malloc memory");
                exit(EXIT_FAILURE);
            }
            strncpy(result[line_count], start, length);
            result[line_count][length] = '\n';     /* Add newline at the end */
            result[line_count][length + 1] = '\0'; /* Null-terminate the line */
            line_count++;

            start = input + 1; /* Move start to the next character */
        }
        input++;
    }

    /* Handle the last line if it doesn't end with a newline */
    if (start != input) {
        if (line_count >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            result = (char **)realloc(result, capacity * sizeof(char *));
            if (result == NULL) {
                perror("Failed to realloc memory");
                // free(result);
                exit(EXIT_FAILURE);
            }
        }

        int length = input - start;
        result[line_count] = (char *)malloc(
            (length + 2) * sizeof(char)); /* +2 for '\n' and '\0' */
        if (result[line_count] == NULL) {
            perror("Failed to malloc memory");
            exit(EXIT_FAILURE);
        }
        strncpy(result[line_count], start, length);
        result[line_count][length] = '\n';     /* Add newline at the end */
        result[line_count][length + 1] = '\0'; /* Null-terminate the line */
        line_count++;
    }

    /* Store the line count in the provided pointer */
    *count = line_count;

    for (int i = 0; i < line_count; i++) {
        printf("split_string_block_into_array: %s", result[i]);
    }

    return result;
}

void test(char *texto) {
    char **texto_processado;
    int initial_line_count;
    token_markdown_t tokens[128];
    int quantidade_tokens = 0;
    int espacos_lista = 0;
    printf("TEXTO: {%s}\n", texto);
    texto_processado =
        split_string_block_into_array(texto, &initial_line_count);
    for (int i = 0; i < initial_line_count; i++) {
        printf("texto_processado: %s", texto_processado[i]);
    }

    for (int i = 0; i < initial_line_count; i++) {
        tokenizar(texto_processado[i], tokens, &quantidade_tokens,
                  &espacos_lista);
    }

    renderizar_tokens_debug(tokens, quantidade_tokens);

    int numero_linhas;
    char **linhas = renderizar_tokens_para_strings(tokens, quantidade_tokens,
                                                   &numero_linhas);

    for (int i = 0; i < numero_linhas; i++) {
        printf("LINHAS: %s", linhas[i]);
    }

    int output_count = 0;
    char **output = split_lines(linhas, numero_linhas, &output_count);
    for (int i = 0; i < output_count; i++) {
        cli_processar_string(output[i]);
    }

    free_dynamic_strings(texto_processado, initial_line_count);
    free_dynamic_strings(linhas, numero_linhas);
    free_dynamic_strings(output, output_count);
}

int main(void) {

    test("# cabeçalho\n"
         "**negrito**\n"
         "*italico*\n"
         "`inline code` *italico* **negrito**\n"
         "**negrito** `inline code` *italico*\n"
         "*italico* **negrito** `inline code`\n"
         "text without a newline "
         "*This is a text with a really big line that spans across more than "
         "80 characteres=that=really=should=never=be split apart*\n"
         "**This is a text with a really big line that spans across more than "
         "80 characteres=that=really=should=never=be split apart**\n"
         "`This is a text with a really big line that spans across more than "
         "80 characteres=that=really=should=never=be split apart`\n"
         "This is a text with á really line that spans across more than 80 characters and some text\n"
         "- a\n"
         "- b\n"
         "  - c\n"
         "  - d\n"
         "  - e\n"
         "    - e\n"
         "  - g\n"
         "> quote\n"
         "> quote\n"
         "> quote\n"
         "> quote\n"
         "```c\n"
         "#include <stdio.h>\n"
         "\n"
         "/* comentario */\n"
         "int main(void) {\n"
         "\tprintf(\"Hello World\\n\");\n"
         "\n"
         "\treturn 0;\n"
         "}\n"
         "```\n"
         "\n");

    return 0;
}

/* Verifica se um caractere inicia um operador */
int iniciar_com_operador(char c) {
    return strchr("+-*/%=&|^~<>!?:.", c) != NULL;
}

/* Encontra o operador mais longo a partir da posição atual */
const char *encontrar_operador(const char *ptr) {
    int i;
    for (i = 0; operadores[i] != NULL; i++) {
        unsigned long len = contar_caracteres_utf8(operadores[i]);
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
            printf("%s/*", obter_codigo_ansi(CIANO_CLARO, 0));
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
                printf("*/%s", obter_codigo_ansi(PADRAO, 0));
                ptr += 2;
            }
            colunas = 0;
            continue;
        }

        /* Lida com diretivas do pré-processador */
        if (*ptr == '#' && (ptr == codigo || *(ptr - 1) == '\n')) {
            if (*ptr == '#' && strncmp(ptr, "#include", 8) == 0 &&
                isspace(*(ptr + 8))) {
                printf("%s#include", obter_codigo_ansi(VERMELHO_CLARO, 0));
                ptr += 8;

                /* Ignora espaços em branco */
                while (isspace(*ptr)) {
                    putchar(*ptr++);
                }
                /* Lida com <...> */
                if (*ptr == '<') {
                    printf("%s<", obter_codigo_ansi(CIANO_CLARO, 0));
                    ptr++;
                    while (*ptr != '\0' && *ptr != '>') {
                        putchar(*ptr++);
                    }
                    if (*ptr == '>') {
                        printf(">%s", obter_codigo_ansi(PADRAO, 0));
                        ptr++;
                    }
                    continue;
                }

                /* Lida com "..." */
                if (*ptr == '"') {
                    printf("%s\"", obter_codigo_ansi(MAGENTA_CLARO, 0));
                    ptr++;
                    while (*ptr != '\0' && *ptr != '"') {
                        putchar(*ptr++);
                    }
                    if (*ptr == '"') {
                        printf("\"%s", obter_codigo_ansi(PADRAO, 0));
                        ptr++;
                    }
                    continue;
                }

            } else {
                printf("%s#", obter_codigo_ansi(VERMELHO_CLARO, 0));
                ptr++;
                while (*ptr != '\0' && *ptr != '\n' &&
                       !(*ptr == '/' && *(ptr + 1) == '*')) {
                    putchar(*ptr++);
                }
                printf(obter_codigo_ansi(PADRAO, 0));
                continue;
            }
        }

        /* Lida com caracteres entre aspas simples */
        if (*ptr == '\'') {
            printf("%s'", obter_codigo_ansi(CIANO_CLARO, 0));
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
                printf("'%s", obter_codigo_ansi(PADRAO, 0));
                ptr++;
            }
            continue;
        }

        /* Lida com strings entre aspas duplas */
        if (*ptr == '"') {
            printf("%s\"", obter_codigo_ansi(VERDE_CLARO, 0));
            ptr++;
            while (*ptr != '\0' && *ptr != '"') {
                if (*ptr == '\\' &&
                    *(ptr + 1) == '"') { /* Lida com aspas duplas escapadas */
                    printf("\\\"");
                    ptr += 2;
                } else {
                    putchar(*ptr++);
                }
            }
            if (*ptr == '"') {
                printf("\"%s", obter_codigo_ansi(PADRAO, 0));
                ptr++;
            }
            continue;
        }

        /* Lida com números */
        if (isdigit(*ptr) || (*ptr == '.' && isdigit(*(ptr + 1)))) {
            printf(obter_codigo_ansi(MAGENTA_CLARO, 0));
            while (isdigit(*ptr) || *ptr == '.') {
                putchar(*ptr++);
            }
            printf(obter_codigo_ansi(PADRAO, 0));
            continue;
        }

        /* Lida com operadores */
        if (iniciar_com_operador(*ptr)) {
            const char *op = encontrar_operador(ptr);
            if (op != NULL) {
                printf("%s%s%s", obter_codigo_ansi(AMARELO_CLARO, 0), op,
                       obter_codigo_ansi(PADRAO, 0));
                ptr += contar_caracteres_utf8(op);
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
                printf("%s%s%s", obter_codigo_ansi(AZUL_CLARO, 0), buffer,
                       obter_codigo_ansi(PADRAO, 0));
            } else {
                printf("%s", buffer);
            }
            continue;
        }

        /* Imprime outros caracteres */
        putchar(*ptr++);
    }
}

char **colorizar_codigo_em_c_str_arr(char *codigo, unsigned long *num_linhas) {
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

    linhas = malloc(capacidade_linhas * sizeof(char *));
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
            char *comentario_inicio =
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
                char *comentario_final =
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
                char *preprocessador_include_inicio = printf_para_string(
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
                    char *include_global_inicio = printf_para_string(
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
                    char *include_local_inicio = printf_para_string(
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
                char *preprocessador_inicio = printf_para_string(
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
            char *string_sq_inicio =
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
                char *string_sq_final =
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
            char *string_dq_inicio = printf_para_string(
                "%s\"", obter_codigo_ansi(VERMELHO_CLARO, 0));
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
                char *string_dq_final =
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
            tamanho_linha += contar_caracteres_utf8(obter_codigo_ansi(MAGENTA_CLARO, 0));
            while (isdigit(*ponteiro) || *ponteiro == '.') {
                strncat(linha_atual, ponteiro++, 1);
                tamanho_linha++;
            }
            strcat(linha_atual, obter_codigo_ansi(PADRAO, 0));
            tamanho_linha += contar_caracteres_utf8(obter_codigo_ansi(PADRAO, 0));
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
                tamanho_linha += contar_caracteres_utf8(op) +
                                 contar_caracteres_utf8(obter_codigo_ansi(AMARELO_CLARO, 0)) +
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
                tamanho_linha += contar_caracteres_utf8(buffer) +
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

void tokenizar(char *linha, token_markdown_t tokens[], int *quantidade_tokens,
               int *espacos_lista) {
    static int em_bloco_codigo = 0;
    char buffer[1024];
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
            while (*language == ' ' ||
                   *language == '\t') { // Skip spaces or tabs
                language++;
            }
            // Extract language (until we hit space, tab, or newline)
            if (*language != '\0' && *language != '\n') {
                char language_buffer[1024];
                int language_c = 0;
                while (*language != ' ' && *language != '\t' &&
                       *language != '\n' && *language != '\0') {
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
    if (((*linha == '-' || *linha == '*') && *(linha + 1) == ' ') ||
        (*linha == ' ') || (*linha == '\t')) {
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
            *espacos_lista =
                identacao_lista; // Set the indentation for list items
        }
        // Avoid division by zero, check if the spaces for list indentation have
        // been set
        if (*espacos_lista > 0 && identacao_lista % *espacos_lista == 0) {
            int nivel_lista = identacao_lista / *espacos_lista;
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = nivel_lista;
            strcpy(tokens[*quantidade_tokens].conteudo,
                   ch); // Save list item content
            (*quantidade_tokens)++;
        } else if (*espacos_lista == 0) {
            // no indentation
            tokens[*quantidade_tokens].tipo = LISTA;
            tokens[*quantidade_tokens].nivel_lista = 0;
            strcpy(tokens[*quantidade_tokens].conteudo,
                   ch); // Save list item content
            (*quantidade_tokens)++;
        }
        return;
    }
    // Process inline formatting (Bold, Italics, etc.)
    char *start_idx;
    while (*ptr) {
        if (*ptr == '#' && (ptr == linha || *(ptr - 1) == ' ')) {
            char title_buffer[1024];
            int title_c = 0;
            ptr += 2;
            title_buffer[title_c] = *" ";
            title_c++;
            while ((*ptr != '\n') && (title_c < 1024 - 1)) {
                title_buffer[title_c] = *ptr;
                ptr++;
                title_c++;
            }

            title_buffer[title_c] = *" ";
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
            // If there's regular text before the bold formatting, process it
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; // Reset the buffer for new content
            }
            // Handle bold formatting: collect bold text
            ptr += 2; // Skip the '**'
            char bold_buffer[1024];
            int bold_c = 0;
            while (!(*ptr == '*' && *(ptr + 1) == '*') && bold_c < 1024 - 1) {
                bold_buffer[bold_c] = *ptr;
                ptr++;
                bold_c++;
            }
            ptr++; // Skip the second '*' character
            bold_buffer[bold_c] = '\0';
            tokens[*quantidade_tokens].tipo = NEGRITO;
            strcpy(tokens[*quantidade_tokens].conteudo, bold_buffer);
            (*quantidade_tokens)++;
        } else if (*ptr == '*' || *ptr == '_') {
            // Italics formatting: collect italic text
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; // Reset the buffer for new content
            }
            ptr++; // Skip the '*' or '_'
            char italic_buffer[1024];
            int italic_c = 0;
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
            // Code formatting: collect inline code
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0'; // Null-terminate the text
                tokens[*quantidade_tokens].tipo = TEXTO;
                strcpy(tokens[*quantidade_tokens].conteudo, buffer);
                (*quantidade_tokens)++;
                indice_buffer = 0; // Reset the buffer for new content
            }
            ptr++; // Skip the '`'
            char codeblock_buffer[1024];
            int codeblock_c = 0;
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
            // Regular text: collect in the buffer
            if (indice_buffer < 1024 - 1) {
                buffer[indice_buffer++] = *ptr;
            }
        }
        ptr++;
    }
    // Process any remaining text
    if (indice_buffer > 0) {
        buffer[indice_buffer] = '\0'; // Null-terminate the text
        tokens[*quantidade_tokens].tipo = TEXTO;
        strcpy(tokens[*quantidade_tokens].conteudo, buffer);
        (*quantidade_tokens)++;
    }
}

void renderizar_tokens(token_markdown_t tokens[], int quantidade_tokens) {
    for (int i = 0; i < quantidade_tokens; i++) {
        renderizar_token(tokens[i]);
    }
}

void renderizar_token(token_markdown_t token) {
    char *modified_text;
    switch (token.tipo) {
    case CABECALHO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    case ITALICO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    case NEGRITO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    case CODIGO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    case BLOCO_CITACAO:
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    case LISTA:
        // Indent based on list level
        for (int j = 0; j < token.nivel_lista; j++) {
            printf("\t");
        }
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
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
        modified_text = substituir_str(token.conteudo, "\n", "\\n");
        printf("%s\n", modified_text);
        break;
    }
}