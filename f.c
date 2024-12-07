#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Test the function */
int main() {
    // Example static string array
    char *static_strings[] = {
        "\033[31mcabeçalho",
        "\033[0m\033[33mnegrito\033[0m",
        "\033[32mitalico\033[0m",
        "\033[34minline code\033[0m \033[32mitalico\033[0m \033[33mnegrito\033[0m",
        "\033[33mnegrito\033[0m \033[34minline code\033[0m \033[32mitalico\033[0m",
        "\033[32mitalico\033[0m \033[33mnegrito\033[0m \033[34minline code\033[0m",
        "\033[33mcode without a newline\033[0m \033[32mThis is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart\033[0m",
        "\033[33mThis is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart\033[0m",
        "\033[34mThis is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart\033[0m",
        "\033[35m- a",
        "\033[0m\033[35m- b",
        "\033[0m	\033[35m- c",
        "\033[0m	\033[35m- d",
        "\033[0m	\033[35m- e",
        "\033[0m		\033[35m- e",
        "\033[0m	\033[35m- g",
        "\033[0m\033[36m> quote",
        "\033[0m\033[36m> quote",
        "\033[0m\033[36m> quote",
        "\033[0m\033[36m> quote",
        "\033[0m\033[1;31m#include \033[1;36m<stdio.h>\033[0m",
        "\n",
        "\033[1;36m/* comentario */\033[0m",
        "\033[1;34mint\033[0m main(\033[1;34mvoid\033[0m) {",
        "	printf(\033[1;32m\"Hello World\\n\"\033[0m);",
        "",
        "	\033[1;34mreturn\033[0m \033[1;35m0\033[0m;",
        "}",
        "\033[37m\033[0m",
        "test test test test test test test test test test test test test test test test test test test test test test test test test test test\n"
    };
    int static_count = sizeof(static_strings) / sizeof(static_strings[0]);

    // Copy the static array to a dynamic array
    char **dynamic_strings = copy_static_to_dynamic(static_strings, static_count);

     int output_count = 0;

    /* Process the input array */
    char **output = split_lines(dynamic_strings, static_count, &output_count);

    /* Print and free the output */
    for (int i = 0; i < output_count; i++) {
        printf("%s\n", output[i]);
        free(output[i]);
    }
    free(output);

    // Free the dynamically allocated array
    free_dynamic_strings(dynamic_strings, static_count);

    return 0;
}
