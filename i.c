#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char **split_string_block_into_array(char *input, int *count) {
    char **result = NULL;     /* Array of strings */
    int capacity = 0;         /* Current capacity of the array */
    int line_count = 0;       /* Number of lines found */
    const char *start = input; /* Pointer to the start of a substring */
    result = calloc(512, sizeof(char*));

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
            result[line_count] = (char *)malloc((length + 2) * sizeof(char)); /* +2 for '\n' and '\0' */
            if (result[line_count] == NULL) {
                perror("Failed to malloc memory");
                exit(EXIT_FAILURE);
            }
            strncpy(result[line_count], start, length);
            result[line_count][length] = '\n';  /* Add newline at the end */
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
        result[line_count] = (char *)malloc((length + 2) * sizeof(char)); /* +2 for '\n' and '\0' */
        if (result[line_count] == NULL) {
            perror("Failed to malloc memory");
            exit(EXIT_FAILURE);
        }
        strncpy(result[line_count], start, length);
        result[line_count][length] = '\n';  /* Add newline at the end */
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

void test(char* texto) {
    char** texto_processado;
    int initial_line_count;
    printf("TEXTO: {%s}\n", texto);
    texto_processado = split_string_block_into_array(texto, &initial_line_count);
    for (int i = 0; i < initial_line_count; i++) {
        printf("texto_processado: %s", texto_processado[i]);
    }

    for (int i = 0; i < initial_line_count; i++) {
        free(texto_processado[i]);
    }
    free(texto_processado);
}

int main(void) {
    test(
        "# cabeçalho\n"
        "**negrito**\n"
        "*italico*\n"
        "`inline code` *italico* **negrito**\n"
        "**negrito** `inline code` *italico*\n"
        "*italico* **negrito** `inline code`\n"
        "**code without a newline** "
        "*This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart*\n"
        "**This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart**\n"
        "`This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart`\n"
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
        "\n"
    );

    return 0;
}
