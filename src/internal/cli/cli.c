#include "internal/cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>

/* Map ANSI escape codes to Windows Console attributes */
WORD get_windows_color(const char* ansi_code) {
    if (strcmp(ansi_code, "30") == 0) return 0;  /* Black */
    if (strcmp(ansi_code, "31") == 0) return FOREGROUND_RED;  /* Red */
    if (strcmp(ansi_code, "32") == 0) return FOREGROUND_GREEN;  /* Green */ 
    if (strcmp(ansi_code, "33") == 0) return FOREGROUND_RED | FOREGROUND_GREEN;  /* Yellow */ 
    if (strcmp(ansi_code, "34") == 0) return FOREGROUND_BLUE;  /* Blue */ 
    if (strcmp(ansi_code, "35") == 0) return FOREGROUND_RED | FOREGROUND_BLUE;  /* Magenta */ 
    if (strcmp(ansi_code, "36") == 0) return FOREGROUND_GREEN | FOREGROUND_BLUE;  /* Cyan */ 
    if (strcmp(ansi_code, "37") == 0) return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;  /* White */ 
    if (strcmp(ansi_code, "90") == 0) return FOREGROUND_INTENSITY;  /* Bright Black (Gray) */ 
    if (strcmp(ansi_code, "91") == 0) return FOREGROUND_INTENSITY | FOREGROUND_RED;  /* Bright Red */ 
    if (strcmp(ansi_code, "92") == 0) return FOREGROUND_INTENSITY | FOREGROUND_GREEN;  /* Bright Green */ 
    if (strcmp(ansi_code, "93") == 0) return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;  /* Bright Yellow */ 
    if (strcmp(ansi_code, "94") == 0) return FOREGROUND_INTENSITY | FOREGROUND_BLUE;  /* Bright Blue */ 
    if (strcmp(ansi_code, "95") == 0) return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;  /* Bright Magenta */ 
    if (strcmp(ansi_code, "96") == 0) return FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;  /* Bright Cyan */ 
    if (strcmp(ansi_code, "97") == 0) return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;  /* Bright White */

    return 7;  /* Default console color */ 
}
#endif

const char* color_codes[] = {
    "\033[0m",  /* 0 DEFAULT */
    "\033[90m", /* 1 LIGHT_BLACK */
    "\033[91m", /* 2 LIGHT_RED */
    "\033[92m", /* 3 LIGHT_GREEN */
    "\033[93m", /* 4 LIGHT_YELLOW */
    "\033[94m", /* 5 LIGHT_BLUE */
    "\033[95m", /* 6 LIGHT_MAGENTA */
    "\033[96m", /* 7 LIGHT_CYAN */
    "\033[97m", /* 8 LIGHT_WHITE */
    "\033[30m", /* 9 DARK_BLACK */
    "\033[31m", /* 10 DARK_RED */
    "\033[32m", /* 11 DARK_GREEN */
    "\033[33m", /* 12 DARK_YELLOW */
    "\033[34m", /* 13 DARK_BLUE */
    "\033[35m", /* 14 DARK_MAGENTA */
    "\033[36m", /* 15 DARK_CYAN */
    "\033[37m"  /* 16 DARK_WHITE */
};

char* color_text(char* string, cli_colors_t color) {
    const char* colorCode;
    unsigned long resultLen;
    char* result;

    colorCode = color_codes[DEFAULT];
    if (color >= 0 && color < (sizeof(color_codes) / sizeof(color_codes[0]))) {
        colorCode = color_codes[color];
    }

    resultLen = strlen(colorCode) + strlen(string) + strlen("\033[0m") + 1;
    result = (char*)malloc(resultLen);

    if (result == NULL) {
        return string; /* Fallback to uncolored text */
    }

    sprintf(result, "%s%s\033[0m", colorCode, string);
    return result;
}

void _display_text(char* text) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD defaultColor = 7;  
    const char* pos = text;

    while (*pos) {
        if (*pos == '\033' && *(pos + 1) == '[') {  
            pos += 2;

            /* Extract the color code */
            char colorCode[3] = {0};
            int i = 0;
            while (*pos >= '0' && *pos <= '9' && i < 2) {
                colorCode[i++] = *pos++;
            }

            /* Skip the 'm' character */
            if (*pos == 'm') {
                pos++;
            }

            /* Set the console color */
            WORD windowsColor = get_windows_color(colorCode);
            SetConsoleTextAttribute(hConsole, windowsColor);
        } else {
            /* Print characters until the next escape sequence or end of string */
            const char* nextEscape = strchr(pos, '\033');
            size_t length = nextEscape ? (size_t)(nextEscape - pos) : strlen(pos);
            fwrite(pos, sizeof(char), length, stdout);
            pos += length;
        }
    }

    /* Reset console color to default */
    SetConsoleTextAttribute(hConsole, defaultColor);

#else
    /* On Linux, just print the text with escape codes */
    printf("%s", text);
#endif
}

char* append_before_index(const char* str1, const char* str2, int idx) {
    int len1, len2;
    char* result;
    if (str1 == NULL || str2 == NULL) {
        printf("Error: One or both strings are NULL\n");
        return NULL;
    }

    len1 = strlen(str1);
    len2 = strlen(str2);

    if (idx < 0 || idx > len1) {
        printf("Error: Index out of bounds\n");
        return NULL;
    }

    result = (char*)malloc(len1 + len2 + 1);
    if (result == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL; 
    }

    memcpy(result, str1, idx);
    memcpy(result + idx, str2, len2);
    memcpy(result + idx + len2, str1 + idx, len1 - idx + 1);

    return result;
}

char* append_after_index(const char* str1, const char* str2, int idx) {
    int len1, len2;
    char* result;
    if (str1 == NULL || str2 == NULL) {
        printf("Error: One or both strings are NULL\n");
        return NULL;
    }

    len1 = strlen(str1);
    len2 = strlen(str2);

    if (idx < 0 || idx >= len1) {
        printf("Error: Index out of bounds\n");
        return NULL;
    }

    result = (char*)malloc(len1 + len2 + 1);
    if (result == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL; 
    }

    memcpy(result, str1, idx + 1);

    memcpy(result + idx + 1, str2, len2);

    memcpy(result + idx + 1 + len2, str1 + idx + 1, len1 - idx);

    result[len1 + len2] = '\0';

    return result;
}

char* remove_char_at_index(const char* str, int index) {
    int len;
    char* result;
    if (str == NULL || index < 0 || (unsigned long)index >= strlen(str)) {
        return NULL;
    }

    len = strlen(str);
    result = (char*)malloc(len);

    if (result == NULL) {
        return NULL; 
    }

    strncpy(result, str, index);

    strcpy(result + index, str + index + 1);

    return result;
}

char* add_before_after_remove_index(const char* str1, int idx1, const char* str_before, int idx2, const char* str_after, int idx3) {
    /* Declarations */
    int len1, len_before, len_after, new_len;
    char* result;
    char* removed_str;

    /* Null checks for input strings */
    if (str1 == NULL || str_before == NULL || str_after == NULL) {
        printf("Error: One or more input strings are NULL\n");
        return NULL;
    }

    removed_str = remove_char_at_index(str1, idx3);

    /* Calculate the lengths of the input strings */
    len1 = strlen(removed_str);
    len_before = strlen(str_before);
    len_after = strlen(str_after);

    /* Validate indices */
    if (idx1 < 0 || idx1 > len1 || idx2 < 0 || idx2 >= len1) {
        printf("Error: Index out of bounds\n");
        return NULL;
    }

    /* Calculate the length of the resulting string */
    new_len = len1 + len_before + len_after;

    /* Allocate memory for the result */
    result = (char*)malloc(new_len + 1); /* +1 for the null terminator */
    if (result == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    /* Copy the first part of removed_str up to idx1 */
    memcpy(result, removed_str, idx1);

    /* Insert str_before before idx1 */
    memcpy(result + idx1, str_before, len_before);

    /* Copy the part of removed_str between idx1 and idx2 (inclusive) */
    memcpy(result + idx1 + len_before, removed_str + idx1, idx2 - idx1 + 1);

    /* Insert str_after after idx2 */
    memcpy(result + idx1 + len_before + (idx2 - idx1 + 1), str_after, len_after);

    /* Copy the remainder of removed_str after idx2 */
    memcpy(result + idx1 + len_before + (idx2 - idx1 + 1) + len_after, removed_str + idx2 + 1, len1 - idx2);

    /* Null-terminate the resulting string */
    result[new_len] = '\0';

    free(removed_str);

    return result;
}

char* get_colored_format_string(const char* str) {
    int i = 0;
    int start_skip_chars = 0;
    char* temp_text = NULL;
    char* result_text = (char*)malloc(strlen(str) + 1);
    unsigned long result_size = strlen(str) + 1;

    if (result_text == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    strcpy(result_text, str);

    while (result_text[i] != '\0') {
        if (result_text[i] == '%') {
            start_skip_chars = i;
            if (result_text[i + 1] == '%') {
                i += 2;
                continue;
            }

            if (isdigit(result_text[i + 1])) {
                while (isdigit(result_text[i + 1])) {
                    i++;
                }
                if (result_text[i + 1] == '$') {
                    i++;
                }
            }

            while (result_text[i + 1] == '-' || result_text[i + 1] == '+' || result_text[i + 1] == ' ' ||
                   result_text[i + 1] == '0' || result_text[i + 1] == '#' || result_text[i + 1] == '\'') {
                i++;
            }

            if (isdigit(result_text[i + 1])) {
                while (isdigit(result_text[i + 1])) {
                    i++;
                }
            } else if (result_text[i + 1] == '*') {
                i++;
            }

            if (result_text[i + 1] == '.') {
                i++;
                if (isdigit(result_text[i + 1])) {
                    while (isdigit(result_text[i + 1])) {
                        i++;
                    }
                } else if (result_text[i + 1] == '*') {
                    i++;
                }
            }

            if (result_text[i + 1] == 'h' || result_text[i + 1] == 'l' || result_text[i + 1] == 'L' ||
                result_text[i + 1] == 'z' || result_text[i + 1] == 'j' || result_text[i + 1] == 't') {
                i++;
                if (result_text[i + 1] == result_text[i]) {
                    i++; 
                }
            }

            if (result_text[i + 1] == 's' || result_text[i + 1] == 'd' || result_text[i + 1] == 'f' || 
                result_text[i + 1] == 'x' || result_text[i + 1] == 'X' || result_text[i + 1] == 'o' ||
                result_text[i + 1] == 'u' || result_text[i + 1] == 'c' || result_text[i + 1] == 'p') {
                
                i++;

                if (isdigit(result_text[i + 1])) {
                    int num = 0;
                    while (isdigit(result_text[i + 1])) {
                        num = num * 10 + (result_text[i + 1] - '0');
                        i++;
                    }
                    temp_text = add_before_after_remove_index(result_text, start_skip_chars, color_codes[num], i-1, color_codes[0], i);
                    result_size = strlen(temp_text) + 1;
                    result_text = realloc(result_text, result_size);
                    if (result_text == NULL) {
                        printf("Error: Memory reallocation failed\n");
                        return NULL;
                    }
                    
                    strcpy(result_text, temp_text);
                    i += strlen(color_codes[num]);
                    free(temp_text);
                }
            }
        } else {
            i++;
        }
    }

    return result_text;
}


char* __vsnprintf(const char *format, va_list args) {
    char* buffer;
    int buffer_size;
    buffer_size = strlen(format) * 2;
    buffer = calloc(buffer_size, sizeof(char));
    vsprintf(buffer, format, args);
    return buffer;
}


char* _color_string(const char* format, va_list args) {
    char* formatted_string = NULL;
    char* format_string;

    format_string = get_colored_format_string(format);
    
    formatted_string = __vsnprintf(format_string, args);
    free(format_string);

    return formatted_string;
}

void display_text(const char* format, ...) {
    va_list args;
    char* final_string;
    va_start(args, format);
    final_string = _color_string(format, args);
    _display_text(final_string);
    va_end(args);
    free(final_string);
}
