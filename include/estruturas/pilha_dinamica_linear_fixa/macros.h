#ifndef ESTRUTURAS_PILHA_ESTATICA_LINEAR_MACROS
#define ESTRUTURAS_PILHA_ESTATICA_LINEAR_MACROS

#include "config/config.h"

/* Essa é uma medida de segurança para impedir que MACRO_PILHA_FIXA_CODIGO
   não seja re-definido caso tenha sido definido anteriormente */
#ifdef MACRO_PILHA_FIXA_CODIGO

/* Esses macros usados através do código para refletir a mudança no tipo da
   estrutura de dados, eles definem as strings de formatação e exibição além de
   guardar o tipo de dado da pilha fixa, pois ele será utilizado tanto dentro
   da struct quanto dentro da função sizeof() para alocação do espaço.

   Esse macro é automaticamente definido quando alterarmos o macro
   MACRO_PILHA_FIXA_CODIGO no arquivo config/config.h */
#if MACRO_PILHA_FIXA_CODIGO == 0
#define MACRO_PILHA_FIXA_TIPO signed char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhd\n"
#define SCANF_FORMAT "%hhd"
#define SHOW_STRING0 "| %-16lu | %-39hhd "
#define SHOW_STRING1 "| %-36lu | %37hhd "
#define SHOW_STRING2 "| %-36hhd "
#define SHOW_STRING3 "| %-76hhd "
#define SHOW_STRING4 "%hhd"
#elif MACRO_PILHA_FIXA_CODIGO == 1
#define MACRO_PILHA_FIXA_TIPO signed short
#define PRINTF_FORMAT_STR "O valor removido foi: %hd\n"
#define SCANF_FORMAT "%hd"
#define SHOW_STRING0 "| %-16lu | %-39hd "
#define SHOW_STRING1 "| %-36lu | %37hd "
#define SHOW_STRING2 "| %-36hd "
#define SHOW_STRING3 "| %-76hd "
#define SHOW_STRING4 "%hd"
#elif MACRO_PILHA_FIXA_CODIGO == 2
#define MACRO_PILHA_FIXA_TIPO signed int
#define PRINTF_FORMAT_STR "O valor removido foi: %d\n"
#define SCANF_FORMAT "%d"
#define SHOW_STRING0 "| %-16lu | %-39d "
#define SHOW_STRING1 "| %-36lu | %37d "
#define SHOW_STRING2 "| %-36d "
#define SHOW_STRING3 "| %-76d "
#define SHOW_STRING4 "%d"
#elif MACRO_PILHA_FIXA_CODIGO == 3
#define MACRO_PILHA_FIXA_TIPO signed long
#define PRINTF_FORMAT_STR "O valor removido foi: %ld\n"
#define SCANF_FORMAT "%ld"
#define SHOW_STRING0 "| %-16lu | %-39ld "
#define SHOW_STRING1 "| %-36lu | %37ld "
#define SHOW_STRING2 "| %-36ld "
#define SHOW_STRING3 "| %-76ld "
#define SHOW_STRING4 "%ld"
#elif MACRO_PILHA_FIXA_CODIGO == 4
#define MACRO_PILHA_FIXA_TIPO unsigned char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhu\n"
#define SCANF_FORMAT "%hhu"
#define SHOW_STRING0 "| %-16lu | %-39hhu "
#define SHOW_STRING1 "| %-36lu | %37hhu "
#define SHOW_STRING2 "| %-36hhu "
#define SHOW_STRING3 "| %-76hhu "
#define SHOW_STRING4 "%hhu"
#elif MACRO_PILHA_FIXA_CODIGO == 5
#define MACRO_PILHA_FIXA_TIPO unsigned short
#define PRINTF_FORMAT_STR "O valor removido foi: %hu\n"
#define SCANF_FORMAT "%hu"
#define SHOW_STRING0 "| %-16lu | %-39hu "
#define SHOW_STRING1 "| %-36lu | %37hu "
#define SHOW_STRING2 "| %-36hu "
#define SHOW_STRING3 "| %-76hu "
#define SHOW_STRING4 "%hu"
#elif MACRO_PILHA_FIXA_CODIGO == 6
#define MACRO_PILHA_FIXA_TIPO unsigned int
#define PRINTF_FORMAT_STR "O valor removido foi: %u\n"
#define SCANF_FORMAT "%u"
#define SHOW_STRING0 "| %-16lu | %-39u "
#define SHOW_STRING1 "| %-36lu | %37u "
#define SHOW_STRING2 "| %-36u "
#define SHOW_STRING3 "| %-76u "
#define SHOW_STRING4 "%u"
#elif MACRO_PILHA_FIXA_CODIGO == 7
#define MACRO_PILHA_FIXA_TIPO unsigned long
#define PRINTF_FORMAT_STR "O valor removido foi: %lu\n"
#define SCANF_FORMAT "%lu"
#define SHOW_STRING0 "| %-16lu | %-39lu "
#define SHOW_STRING1 "| %-36lu | %37lu "
#define SHOW_STRING2 "| %-36lu "
#define SHOW_STRING3 "| %-76lu "
#define SHOW_STRING4 "%lu"
#elif MACRO_PILHA_FIXA_CODIGO == 8
#define MACRO_PILHA_FIXA_TIPO char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhd\n"
#define SCANF_FORMAT "%hhd"
#define SHOW_STRING0 "| %-16lu | %-39hhd "
#define SHOW_STRING1 "| %-36lu | %37hhd "
#define SHOW_STRING2 "| %-36hhd "
#define SHOW_STRING3 "| %-76hhd "
#define SHOW_STRING4 "%hhd"
#elif MACRO_PILHA_FIXA_CODIGO == 9
#define MACRO_PILHA_FIXA_TIPO short
#define PRINTF_FORMAT_STR "O valor removido foi: %hd\n"
#define SCANF_FORMAT "%hd"
#define SHOW_STRING0 "| %-16lu | %-39hd "
#define SHOW_STRING1 "| %-36lu | %37hd "
#define SHOW_STRING2 "| %-36hd "
#define SHOW_STRING3 "| %-76hd "
#define SHOW_STRING4 "%hd"
#elif MACRO_PILHA_FIXA_CODIGO == 10
#define MACRO_PILHA_FIXA_TIPO int
#define PRINTF_FORMAT_STR "O valor removido foi: %d\n"
#define SCANF_FORMAT "%d"
#define SHOW_STRING0 "| %-16lu | %-39d "
#define SHOW_STRING1 "| %-36lu | %37d "
#define SHOW_STRING2 "| %-36d "
#define SHOW_STRING3 "| %-76d "
#define SHOW_STRING4 "%d"
#elif MACRO_PILHA_FIXA_CODIGO == 11
#define MACRO_PILHA_FIXA_TIPO long
#define PRINTF_FORMAT_STR "O valor removido foi: %ld\n"
#define SCANF_FORMAT "%ld"
#define SHOW_STRING0 "| %-16lu | %-39ld "
#define SHOW_STRING1 "| %-36lu | %37ld "
#define SHOW_STRING2 "| %-36ld "
#define SHOW_STRING3 "| %-76ld "
#define SHOW_STRING4 "%ld"
#elif MACRO_PILHA_FIXA_CODIGO == 12
#define MACRO_PILHA_FIXA_TIPO float
#define PRINTF_FORMAT_STR "O valor removido foi: %f\n"
#define SCANF_FORMAT "%f"
#define SHOW_STRING0 "| %-16lu | %-39f "
#define SHOW_STRING1 "| %-36lu | %37f "
#define SHOW_STRING2 "| %-36f "
#define SHOW_STRING3 "| %-76f "
#define SHOW_STRING4 "%f"
#elif MACRO_PILHA_FIXA_CODIGO == 13
#define MACRO_PILHA_FIXA_TIPO double
#define PRINTF_FORMAT_STR "O valor removido foi: %f\n"
#define SCANF_FORMAT "%f"
#define SHOW_STRING0 "| %-16lu | %-39f "
#define SHOW_STRING1 "| %-36lu | %37f "
#define SHOW_STRING2 "| %-36f "
#define SHOW_STRING3 "| %-76f "
#define SHOW_STRING4 "%f"
#else
#error "Tipo de MACRO_PILHA_FIXA_TIPO não reconhecido"
#error "Tipo Não Reconhecido para SCANF_FORMAT"
#error "Tipo Não Reconhecido para MACRO_PILHA_FIXA_TIPO"
#error "Tipo Não Reconhecido para PRINTF_FORMAT_STR"
#endif
#endif

#endif /* ESTRUTURAS_PILHA_ESTATICA_LINEAR_MACROS */
