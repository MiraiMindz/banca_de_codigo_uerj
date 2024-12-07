#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Define os códigos de cor ANSI */
#define COR_RESET "\033[0m"
#define COR_PALAVRA_CHAVE "\033[1;34m"   /* Azul Brilhante */
#define COR_COMENTARIO "\033[1;31m"   /* Vermelho Brilhante */
#define COR_STRING "\033[1;32m"    /* Verde Brilhante */
#define COR_CARACTERE "\033[1;36m"      /* Ciano Brilhante */
#define COR_PREPROCESSADOR "\033[1;33m" /* Amarelo Brilhante */
#define COR_NUMERO "\033[1;35m"    /* Magenta Brilhante */
#define COR_OPERADOR "\033[1;33m" /* Amarelo Brilhante */
#define COR_INCLUDE_ANGULAR "\033[1;36m" /* Ciano Brilhante para <...> */
#define COR_INCLUDE_ANTIVIRGULA "\033[1;35m" /* Magenta Brilhante para "..." */

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

    while (*ptr != '\0') {
        /* Lida com comentários */
        if (*ptr == '/' && *(ptr + 1) == '*') {
            printf(COR_COMENTARIO "/*");
            ptr += 2;
            while (*ptr != '\0' && !(*ptr == '*' && *(ptr + 1) == '/')) {
                putchar(*ptr++);
            }
            if (*ptr == '*' && *(ptr + 1) == '/') {
                printf("*/" COR_RESET);
                ptr += 2;
            }
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


int main(void) {
    const char *code =
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <ctype.h>\n"
        "\n"
        "/* Define os códigos de cor ANSI */\n"
        "#define COR_RESET \"\\033[0m\"\n"
        "#define COR_PALAVRA_CHAVE \"\\033[1;34m\"   /* Azul Brilhante */\n"
        "#define COR_COMENTARIO \"\\033[1;32m\"   /* Verde Brilhante */\n"
        "#define COR_STRING \"\\033[1;31m\"    /* Vermelho Brilhante */\n"
        "#define COR_CARACTERE \"\\033[1;36m\"      /* Ciano Brilhante */\n"
        "#define COR_PREPROCESSADOR \"\\033[1;33m\" /* Amarelo Brilhante */\n"
        "#define COR_NUMERO \"\\033[1;35m\"    /* Magenta Brilhante */\n"
        "#define COR_OPERADOR \"\\033[1;33m\" /* Amarelo Brilhante */\n"
        "#define COR_INCLUDE_ANGULAR \"\\033[1;36m\" /* Ciano Brilhante para <...> */\n"
        "#define COR_INCLUDE_ANTIVIRGULA \"\\033[1;35m\" /* Magenta Brilhante para \"...\" */\n"
        "\n"
        "/* Lista de palavras-chave do C89 */\n"
        "const char *palavras_chave[] = {\n"
        "    \"auto\", \"break\", \"case\", \"char\", \"const\", \"continue\", \"default\", \"do\", \n"
        "    \"double\", \"else\", \"enum\", \"extern\", \"float\", \"for\", \"goto\", \"if\", \n"
        "    \"int\", \"long\", \"register\", \"return\", \"short\", \"signed\", \"sizeof\", \n"
        "    \"static\", \"struct\", \"switch\", \"typedef\", \"union\", \"unsigned\", \"void\", \n"
        "    \"volatile\", \"while\", NULL\n"
        "};\n"
        "\n"
        "const char *operadores[] = {\n"
        "    \"+\", \"-\", \"*\", \"/\", \"%\", \"=\", \"==\", \"!=\", \"<\", \">\", \"<=\", \">=\", \"&&\", \n"
        "    \"||\", \"!\", \"&\", \"|\", \"^\", \"~\", \"<<\", \">>\", \"++\", \"--\", \"->\", \".\", \n"
        "    \"?\", \":\", NULL\n"
        "};\n"
        "\n"
        "/* Verifica se um caractere inicia um operador */\n"
        "int iniciar_com_operador(char c) {\n"
        "    return strchr(\"+-*/%=&|^~<>!?:.\", c) != NULL;\n"
        "}\n"
        "\n"
        "/* Encontra o operador mais longo a partir da posição atual */\n"
        "const char* encontrar_operador(const char *ptr) {\n"
        "    int i;\n"
        "    for (i = 0; operadores[i] != NULL; i++) {\n"
        "        unsigned long len = strlen(operadores[i]);\n"
        "        if (strncmp(ptr, operadores[i], len) == 0) {\n"
        "            return operadores[i];\n"
        "        }\n"
        "    }\n"
        "    return NULL;\n"
        "}\n"
        "\n"
        "/* Função auxiliar para verificar se uma palavra é uma palavra-chave do C89 */\n"
        "int encontrar_palavra_chave(const char *palavra) {\n"
        "    int i;\n"
        "    for (i = 0; palavras_chave[i] != NULL; i++) {\n"
        "        if (strcmp(palavra, palavras_chave[i]) == 0) {\n"
        "            return 1;\n"
        "        }\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
        "\n"
        "/* Função para destacar a sintaxe do C89 */\n"
        "void colorizar_codigo_em_c(const char *codigo) {\n"
        "    const char *ptr = codigo;\n"
        "    char buffer[256];\n"
        "    int i;\n"
        "\n"
        "    while (*ptr != \'\\0\') {\n"
        "        /* Lida com comentários */\n"
        "        if (*ptr == \'/\' && *(ptr + 1) == \'*\') {\n"
        "            printf(COR_COMENTARIO \"/*\");\n"
        "            ptr += 2;\n"
        "            while (*ptr != \'\\0\' && !(*ptr == \'*\' && *(ptr + 1) == \'/\')) {\n"
        "                putchar(*ptr++);\n"
        "            }\n"
        "            if (*ptr == \'*\' && *(ptr + 1) == \'/\') {\n"
        "                printf(\"*/\" COR_RESET);\n"
        "                ptr += 2;\n"
        "            }\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        /* Lida com diretivas do pré-processador */\n"
        "        if (*ptr == \'#\' && (ptr == codigo || *(ptr - 1) == \'\\n\')) {\n"
        "            if (*ptr == \'#\' && strncmp(ptr, \"#include\", 8) == 0 && isspace(*(ptr + 8))) {\n"
        "                printf(COR_PREPROCESSADOR \"#include\");\n"
        "                ptr += 8;\n"
        "\n"
        "                /* Ignora espaços em branco */\n"
        "                while (isspace(*ptr)) {\n"
        "                    putchar(*ptr++);\n"
        "                }\n"
        "                /* Lida com <...> */\n"
        "                if (*ptr == \'<\') {\n"
        "                    printf(COR_INCLUDE_ANGULAR \"<\");\n"
        "                    ptr++;\n"
        "                    while (*ptr != \'\\0\' && *ptr != \'>\') {\n"
        "                        putchar(*ptr++);\n"
        "                    }\n"
        "                    if (*ptr == \'>\') {\n"
        "                        printf(\">\" COR_RESET);\n"
        "                        ptr++;\n"
        "                    }\n"
        "                    continue;\n"
        "                }\n"
        "\n"
        "                /* Lida com \"...\" */\n"
        "                if (*ptr == \'\"\') {\n"
        "                    printf(COR_INCLUDE_ANTIVIRGULA \"\\\"\");\n"
        "                    ptr++;\n"
        "                    while (*ptr != \'\\0\' && *ptr != \'\"\') {\n"
        "                        putchar(*ptr++);\n"
        "                    }\n"
        "                    if (*ptr == \'\"\') {\n"
        "                        printf(\"\\\"\" COR_RESET);\n"
        "                        ptr++;\n"
        "                    }\n"
        "                    continue;\n"
        "                }\n"
        "\n"
        "            } else {\n"
        "                printf(COR_PREPROCESSADOR \"#\");\n"
        "                ptr++;\n"
        "                while (*ptr != \'\\0\' && *ptr != \'\\n\' && !(*ptr == \'/\' && *(ptr + 1) == \'*\')) {\n"
        "                    putchar(*ptr++);\n"
        "                }\n"
        "                printf(COR_RESET);\n"
        "                continue;\n"
        "            }\n"
        "        }\n"
        "\n"
        "        /* Lida com caracteres entre aspas simples */\n"
        "        if (*ptr == \'\'\') {\n"
        "            printf(COR_CARACTERE \"\'\");\n"
        "            ptr++;\n"
        "            if (*ptr == \'\\\\\') { /* Lida com caractere escapado */\n"
        "                putchar(*ptr++);\n"
        "                if (*ptr != \'\\0\') {\n"
        "                    putchar(*ptr++);\n"
        "                }\n"
        "            } else if (*ptr != \'\\0\') {\n"
        "                putchar(*ptr++);\n"
        "            }\n"
        "            if (*ptr == \'\'\') {\n"
        "                printf(\"\'\" COR_RESET);\n"
        "                ptr++;\n"
        "            }\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        /* Lida com strings entre aspas duplas */\n"
        "        if (*ptr == \'\"\') {\n"
        "            printf(COR_STRING \"\\\"\");\n"
        "            ptr++;\n"
        "            while (*ptr != \'\\0\' && *ptr != \'\"\') {\n"
        "                if (*ptr == \'\\\\\' && *(ptr + 1) == \'\"\') { /* Lida com aspas duplas escapadas */\n"
        "                    printf(\"\\\"\");\n"
        "                    ptr += 2;\n"
        "                } else {\n"
        "                    putchar(*ptr++);\n"
        "                }\n"
        "            }\n"
        "            if (*ptr == \'\"\') {\n"
        "                printf(\"\\\"\" COR_RESET);\n"
        "                ptr++;\n"
        "            }\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        /* Lida com números */\n"
        "        if (isdigit(*ptr) || (*ptr == \'.\' && isdigit(*(ptr + 1)))) {\n"
        "            printf(COR_NUMERO);\n"
        "            while (isdigit(*ptr) || *ptr == \'.\') {\n"
        "                putchar(*ptr++);\n"
        "            }\n"
        "            printf(COR_RESET);\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "         /* Lida com operadores */\n"
        "        if (iniciar_com_operador(*ptr)) {\n"
        "            const char *op = encontrar_operador(ptr);\n"
        "            if (op != NULL) {\n"
        "                printf(COR_OPERADOR \"%s\" COR_RESET, op);\n"
        "                ptr += strlen(op);\n"
        "                continue;\n"
        "            }\n"
        "        }\n"
        "\n"
        "        /* Lida com palavras-chave e identificadores */\n"
        "        if (isalpha(*ptr) || *ptr == \'_\') {\n"
        "            i = 0;\n"
        "            while (isalnum(*ptr) || *ptr == \'_\') {\n"
        "                buffer[i++] = *ptr++;\n"
        "            }\n"
        "            buffer[i] = \'\\0\';\n"
        "            if (encontrar_palavra_chave(buffer)) {\n"
        "                printf(COR_PALAVRA_CHAVE \"%s\" COR_RESET, buffer);\n"
        "            } else {\n"
        "                printf(\"%s\", buffer);\n"
        "            }\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        /* Imprime outros caracteres */\n"
        "        putchar(*ptr++);\n"
        "    }\n"
        "}\n"
        "\n"
        "\n"
        "int main(void) {\n"
        "    const char *code =\n"
        "        \"/* Example program demonstrating all ANSI C89 syntax elements */\\n\"\n"
        "        \"\\n\"\n"
        "        \"#include \\\"internal.h\\\" /* internal header */\\n\"\n"
        "        \"#include <stdio.h> /* Preprocessor directive for standard I/O */\\n\"\n"
        "        \"#include <stdlib.h> /* Preprocessor directive for general utilities */\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Macro definition */\\n\"\n"
        "        \"#define MAX(a, b) ((a) > (b) ? (a) : (b))\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Typedef for renaming a type */\\n\"\n"
        "        \"typedef unsigned int uint;\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Enumeration definition */\\n\"\n"
        "        \"enum Color {\\n\"\n"
        "        \"    RED,\\n\"\n"
        "        \"    GREEN,\\n\"\n"
        "        \"    BLUE\\n\"\n"
        "        \"};\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Structure definition */\\n\"\n"
        "        \"struct Point {\\n\"\n"
        "        \"    int x;\\n\"\n"
        "        \"    int y;\\n\"\n"
        "        \"};\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Union definition */\\n\"\n"
        "        \"union Data {\\n\"\n"
        "        \"    int i;\\n\"\n"
        "        \"    float f;\\n\"\n"
        "        \"};\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Function prototype */\\n\"\n"
        "        \"void displayMessage(const char *message);\\n\"\n"
        "        \"int factorial(int n);\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Global variable declaration */\\n\"\n"
        "        \"int globalVar = 42;\\n\"\n"
        "        \"\\n\"\n"
        "        \"int main(void) {\\n\"\n"
        "        \"    /* Local variable declarations */\\n\"\n"
        "        \"    int i, j;\\n\"\n"
        "        \"    uint unsignedNum = 100;\\n\"\n"
        "        \"    enum Color favoriteColor = GREEN;\\n\"\n"
        "        \"    struct Point p1 = {10, 20};\\n\"\n"
        "        \"    union Data data;\\n\"\n"
        "        \"    \\n\"\n"
        "        \"    /* Array declaration and initialization */\\n\"\n"
        "        \"    int numbers[5] = {1, 2, 3, 4, 5};\\n\"\n"
        "        \"    \\n\"\n"
        "        \"    /* Pointer and memory allocation */\\n\"\n"
        "        \"    int *dynamicArray = (int *)malloc(5 * sizeof(int));\\n\"\n"
        "        \"    if (dynamicArray == NULL) {\\n\"\n"
        "        \"        fprintf(stderr, \\\"Memory allocation failed\\n\\\");\\n\"\n"
        "        \"        return 1; /* Exit with error code */\\n\"\n"
        "        \"    }\\n\"\n"
        "        \"    \\n\"\n"
        "        \"    for (i = 0; i < 5; i++) {\\n\"\n"
        "        \"        dynamicArray[i] = numbers[i] * 2;\\n\"\n"
        "        \"    }\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Print values using loops */\\n\"\n"
        "        \"    printf(\\\"Numbers multiplied by 2: \\\");\\n\"\n"
        "        \"    for (i = 0; i < 5; i++) {\\n\"\n"
        "        \"        printf(\\\"%d \\\", dynamicArray[i]);\\n\"\n"
        "        \"    }\\n\"\n"
        "        \"    printf(\\\"\\n\\\");\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Call a macro */\\n\"\n"
        "        \"    printf(\\\"Max of %d and %d is %d\\n\\\", numbers[1], numbers[3], MAX(numbers[1], numbers[3]));\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Call a function */\\n\"\n"
        "        \"    displayMessage(\\\"Hello from a function!\\\");\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Use a union */\\n\"\n"
        "        \"    data.i = 10;\\n\"\n"
        "        \"    printf(\\\"Union integer value: %d\\n\\\", data.i);\\n\"\n"
        "        \"    data.f = 5.75f;\\n\"\n"
        "        \"    printf(\\\"Union float value: %.2f\\n\\\", data.f);\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Call a recursive function */\\n\"\n"
        "        \"    printf(\\\"Factorial of 5 is: %d\\n\\\", factorial(5));\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Use a switch statement */\\n\"\n"
        "        \"    switch (favoriteColor) {\\n\"\n"
        "        \"        case RED:\\n\"\n"
        "        \"            printf(\\\"Favorite color is red.\\n\\\");\\n\"\n"
        "        \"            break;\\n\"\n"
        "        \"        case GREEN:\\n\"\n"
        "        \"            printf(\\\"Favorite color is green.\\n\\\");\\n\"\n"
        "        \"            break;\\n\"\n"
        "        \"        case BLUE:\\n\"\n"
        "        \"            printf(\\\"Favorite color is blue.\\n\\\");\\n\"\n"
        "        \"            break;\\n\"\n"
        "        \"        default:\\n\"\n"
        "        \"            printf(\\\"Unknown color.\\n\\\");\\n\"\n"
        "        \"    }\\n\"\n"
        "        \"\\n\"\n"
        "        \"    /* Free dynamically allocated memory */\\n\"\n"
        "        \"    free(/* comment */dynamicArray);\\n\"\n"
        "        \"\\n\"\n"
        "        \"    return 0; /* Successful program termination */\\n\"\n"
        "        \"}\\n\"\n"
        "        \"\\n\"\n"
        "        \"/* Function definitions */\\n\"\n"
        "        \"void displayMessage(const char *message) {\\n\"\n"
        "        \"    printf(\\\"%s\\n\\\", message);\\n\"\n"
        "        \"}\\n\"\n"
        "        \"\\n\"\n"
        "        \"int factorial(int n) {\\n\"\n"
        "        \"    if (n /* comment */ <= 1) {\\n\"\n"
        "        \"        return 1;\\n\"\n"
        "        \"    }\\n\"\n"
        "        \"    return /* comment */ n * factorial(n - 1);\\n\"\n"
        "        \"}\\n\"\n"
        "        \"/* Example demonstrating character literals and strings */\\n\"\n"
        "        \"char c = \'a\';\\n\"\n"
        "        \"char escaped = \'\\n\';\\n\"\n"
        "        \"char single_quote = \'\\\'\';\\n\"\n"
        "        \"char *str1 = \\\"Hello, World!\\\";\\n\"\n"
        "        \"char *str2 = \\\"He said, \'Hello!\'\\\";\\n\"\n"
        "        \"char *str3 = \\\"Nested \\\"quotes\\\" and \'single quotes\' inside.\\\";\\n\";\n"
        "\n"
        "    colorizar_codigo_em_c(code);\n"
        "    return 0;\n"
        "}\n"
        "\n";

    colorizar_codigo_em_c(code);
    return 0;
}
