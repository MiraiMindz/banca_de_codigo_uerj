#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to split a string at '\n' into an array of strings */
char **split_string(char *input, int *count) {
    char **result = NULL;     /* Array of strings */
    int capacity = 0;         /* Current capacity of the array */
    int line_count = 0;       /* Number of lines found */
    char *start = input; /* Pointer to the start of a substring */

    while (*input != '\0') {
        if (*input == '\n') {
            /* Allocate space for the array if needed */
            if (line_count >= capacity) {
                capacity = (capacity == 0) ? 1 : capacity * 2;
                result = (char **)realloc(result, capacity * sizeof(char *));
                if (result == NULL) {
                    perror("Failed to realloc memory");
                    exit(EXIT_FAILURE);
                }
            }

            /* Allocate and copy the current line */
            int length = input - start;
            result[line_count] = (char *)malloc((length + 1) * sizeof(char));
            if (result[line_count] == NULL) {
                perror("Failed to malloc memory");
                exit(EXIT_FAILURE);
            }
            strncpy(result[line_count], start, length);
            result[line_count][length] = '\0'; /* Null-terminate the line */
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
                exit(EXIT_FAILURE);
            }
        }

        int length = input - start;
        result[line_count] = (char *)malloc((length + 1) * sizeof(char));
        if (result[line_count] == NULL) {
            perror("Failed to malloc memory");
            exit(EXIT_FAILURE);
        }
        strncpy(result[line_count], start, length);
        result[line_count][length] = '\0';
        line_count++;
    }

    /* Store the line count in the provided pointer */
    *count = line_count;

    return result;
}

/* Example usage */
int main() {
    char *test_string = "# cabeçalho\n"
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
        "\n";
    int num_lines = 0;
    char **lines = split_string(test_string, &num_lines);

    printf("Split result:\n");
    for (int i = 0; i < num_lines; i++) {
        printf("Line %d: %s\n", i + 1, lines[i]);
        free(lines[i]); /* Free each line */
    }
    free(lines); /* Free the array */

    return 0;
}
