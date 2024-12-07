#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *join_strings(char **strings, int count) {
    if (strings == NULL || count <= 0) {
        return NULL; // Return NULL for invalid input.
    }

    // Step 1: Calculate total size required
    size_t total_length = 0;
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            total_length += strlen(strings[i]);
        }
    }
    total_length += count - 1; // Add space for '\n' delimiters
    total_length += 1;         // Add space for null terminator

    // Step 2: Allocate memory for the result
    char *result = (char *)malloc(total_length * sizeof(char));
    if (result == NULL) {
        return NULL; // Return NULL if memory allocation fails.
    }

    // Step 3: Concatenate strings with '\n' as delimiter
    result[0] = '\0'; // Initialize the string
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            strcat(result, strings[i]);
        }
        if (i < count - 1) {
            strcat(result, "\n"); // Add '\n' between strings
        }
    }

    return result;
}

int main() {
    // Example usage
    char *strings[] = {"Hello", "World", "This", "Is", "ANSI C"};
    int count = sizeof(strings) / sizeof(strings[0]);

    char *result = join_strings(strings, count);
    if (result != NULL) {
        printf("Joined String:\n%s\n", result);
        free(result); // Free allocated memory
    } else {
        printf("Failed to join strings.\n");
    }

    return 0;
}
