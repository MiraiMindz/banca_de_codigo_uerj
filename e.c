#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COLUMNS 80

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
int escape_sequence_length(char *str, int start) {
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
void split_line(char *line, char ***result, int *result_count) {
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

        if (visible_count > MAX_COLUMNS) {
            int split_at = (last_space != -1) ? last_space : i - 1;
            int chunk_length = split_at - start + 1;
            char *chunk = (char *)malloc(chunk_length + 1);
            strncpy(chunk, line + start, chunk_length);
            chunk[chunk_length] = '\0';
            *result = (char **)realloc(*result, (*result_count + 1) * sizeof(char *));
            (*result)[*result_count] = chunk;
            (*result_count)++;

            start = split_at + 1;
            visible_count = i - start;
            last_space = -1;
        }
    }

    if (start < len) {
        char *chunk = duplicar_string(line + start);
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


/* Test the function */
int main() {
    
    char *static_strings[] = {
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

    // Calculate the number of strings
    size_t num_strings = sizeof(static_strings) / sizeof(static_strings[0]);

    // Allocate memory for the array of pointers
    char **entrada = calloc(num_strings, sizeof(char *));
    if (!entrada) {
        perror("Failed to allocate memory for the outer array");
        return 1;
    }

    // Allocate memory for each string and copy the content
    for (size_t i = 0; i < num_strings; ++i) {
        size_t len = strlen(static_strings[i]) + 1; // Include null terminator
        entrada[i] = calloc(len, sizeof(char));
        if (!entrada[i]) {
            perror("Failed to allocate memory for a string");
            // Free previously allocated memory before exiting
            for (size_t j = 0; j < i; ++j) {
                free(entrada[j]);
            }
            free(entrada);
            return 1;
        }
        strcpy(entrada[i], static_strings[i]);
    }

    int input_count = sizeof(entrada) / sizeof(entrada[0]);
    int output_count = 0;

    char **output = split_lines(entrada, num_strings, &output_count);

    for (int i = 0; i < output_count; i++) {
        printf("%d: %s\n", i, output[i]);
        free(output[i]);
    }
    free(output);

    return 0;
}
