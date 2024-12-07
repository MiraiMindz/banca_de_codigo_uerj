a = """
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Define os códigos de cor ANSI */
#define COR_RESET "\033[0m"
#define COR_PALAVRA_CHAVE "\033[1;34m"   /* Azul Brilhante */
#define COR_COMENTARIO "\033[1;32m"   /* Verde Brilhante */
#define COR_STRING "\033[1;31m"    /* Vermelho Brilhante */
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
        if (*ptr == '#' && (ptr == codigo || *(ptr - 1) == '\\n')) {
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
                while (*ptr != '\0' && *ptr != '\\n' && !(*ptr == '/' && *(ptr + 1) == '*')) {
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
            if (*ptr == '\\\\') { /* Lida com caractere escapado */
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
                if (*ptr == '\\\\' && *(ptr + 1) == '\"') { /* Lida com aspas duplas escapadas */
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
        "/* Example program demonstrating all ANSI C89 syntax elements */\\n"
        "\\n"
        "#include \"internal.h\" /* internal header */\\n"
        "#include <stdio.h> /* Preprocessor directive for standard I/O */\\n"
        "#include <stdlib.h> /* Preprocessor directive for general utilities */\\n"
        "\\n"
        "/* Macro definition */\\n"
        "#define MAX(a, b) ((a) > (b) ? (a) : (b))\\n"
        "\\n"
        "/* Typedef for renaming a type */\\n"
        "typedef unsigned int uint;\\n"
        "\\n"
        "/* Enumeration definition */\\n"
        "enum Color {\\n"
        "    RED,\\n"
        "    GREEN,\\n"
        "    BLUE\\n"
        "};\\n"
        "\\n"
        "/* Structure definition */\\n"
        "struct Point {\\n"
        "    int x;\\n"
        "    int y;\\n"
        "};\\n"
        "\\n"
        "/* Union definition */\\n"
        "union Data {\\n"
        "    int i;\\n"
        "    float f;\\n"
        "};\\n"
        "\\n"
        "/* Function prototype */\\n"
        "void displayMessage(const char *message);\\n"
        "int factorial(int n);\\n"
        "\\n"
        "/* Global variable declaration */\\n"
        "int globalVar = 42;\\n"
        "\\n"
        "int main(void) {\\n"
        "    /* Local variable declarations */\\n"
        "    int i, j;\\n"
        "    uint unsignedNum = 100;\\n"
        "    enum Color favoriteColor = GREEN;\\n"
        "    struct Point p1 = {10, 20};\\n"
        "    union Data data;\\n"
        "    \\n"
        "    /* Array declaration and initialization */\\n"
        "    int numbers[5] = {1, 2, 3, 4, 5};\\n"
        "    \\n"
        "    /* Pointer and memory allocation */\\n"
        "    int *dynamicArray = (int *)malloc(5 * sizeof(int));\\n"
        "    if (dynamicArray == NULL) {\\n"
        "        fprintf(stderr, \"Memory allocation failed\\n\");\\n"
        "        return 1; /* Exit with error code */\\n"
        "    }\\n"
        "    \\n"
        "    for (i = 0; i < 5; i++) {\\n"
        "        dynamicArray[i] = numbers[i] * 2;\\n"
        "    }\\n"
        "\\n"
        "    /* Print values using loops */\\n"
        "    printf(\"Numbers multiplied by 2: \");\\n"
        "    for (i = 0; i < 5; i++) {\\n"
        "        printf(\"%d \", dynamicArray[i]);\\n"
        "    }\\n"
        "    printf(\"\\n\");\\n"
        "\\n"
        "    /* Call a macro */\\n"
        "    printf(\"Max of %d and %d is %d\\n\", numbers[1], numbers[3], MAX(numbers[1], numbers[3]));\\n"
        "\\n"
        "    /* Call a function */\\n"
        "    displayMessage(\"Hello from a function!\");\\n"
        "\\n"
        "    /* Use a union */\\n"
        "    data.i = 10;\\n"
        "    printf(\"Union integer value: %d\\n\", data.i);\\n"
        "    data.f = 5.75f;\\n"
        "    printf(\"Union float value: %.2f\\n\", data.f);\\n"
        "\\n"
        "    /* Call a recursive function */\\n"
        "    printf(\"Factorial of 5 is: %d\\n\", factorial(5));\\n"
        "\\n"
        "    /* Use a switch statement */\\n"
        "    switch (favoriteColor) {\\n"
        "        case RED:\\n"
        "            printf(\"Favorite color is red.\\n\");\\n"
        "            break;\\n"
        "        case GREEN:\\n"
        "            printf(\"Favorite color is green.\\n\");\\n"
        "            break;\\n"
        "        case BLUE:\\n"
        "            printf(\"Favorite color is blue.\\n\");\\n"
        "            break;\\n"
        "        default:\\n"
        "            printf(\"Unknown color.\\n\");\\n"
        "    }\\n"
        "\\n"
        "    /* Free dynamically allocated memory */\\n"
        "    free(/* comment */dynamicArray);\\n"
        "\\n"
        "    return 0; /* Successful program termination */\\n"
        "}\\n"
        "\\n"
        "/* Function definitions */\\n"
        "void displayMessage(const char *message) {\\n"
        "    printf(\"%s\\n\", message);\\n"
        "}\\n"
        "\\n"
        "int factorial(int n) {\\n"
        "    if (n /* comment */ <= 1) {\\n"
        "        return 1;\\n"
        "    }\\n"
        "    return /* comment */ n * factorial(n - 1);\\n"
        "}\\n"
        "/* Example demonstrating character literals and strings */\\n"
        "char c = 'a';\\n"
        "char escaped = '\\n';\\n"
        "char single_quote = '\\'';\\n"
        "char *str1 = \"Hello, World!\";\\n"
        "char *str2 = \"He said, 'Hello!'\";\\n"
        "char *str3 = \"Nested \\\"quotes\\\" and 'single quotes' inside.\";\\n";

    colorizar_codigo_em_c(code);
    return 0;
}
"""

for i in a.split("\n"):
    i = i.replace("\\", "\\\\")
    i = i.replace("\n", "\\n")
    i = i.replace("\"", "\\\"")
    i = i.replace("\'", "\\\'")
    i = i.replace("\033", "\\033")
    i = i.replace("\0", "\\0")
    print(f"\"{i}\\n\"")
