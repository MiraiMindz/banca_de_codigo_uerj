/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
 *
 * Esse arquivo em específico é o arquivo de configuração do codigo, ele define
 * os tipos de dado, e o comportamento do código em geral.
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

/* Explicitamente definimos NULL aqui pois o ambiente pode ou não ter uma
 * definição padrão para NULL, e também para caso o usuário queira definir um
 * valor diferente para NULL */
#ifndef NULL
#define NULL ((void *)0)
#endif

/* Esse macro permite o uso de cores no terminal, colorizando todas as saidas
 * 1 significa SIM, 0 significa NÃO; qualquer valor diferente pode ter 
 * comportamento inesperado */
#ifndef _USE_COLOR
#define _USE_COLOR 0
#endif

/* Esse código é usado nas diretrizes de pré-processador para dinamicamente
   adaptar o código da fila linear fixa para o tipo de dado selecionado (Em C 
   não conseguimos fazer comparação de textos em macros, somente números) a 
   baixo você encontra a tabela com os códigos para cada tipo:
        0 - signed char   4 - unsigned char   8  - char  12 - float
        1 - signed short  5 - unsigned short  9  - short 13 - double
        2 - signed int    6 - unsigned int    10 - int
        3 - signed long   7 - unsigned long   11 - long
        
    o #ifndef é uma medida de segurança para impedir que 
    MACRO_FILA_FIXA_CODIGO seja re-definido caso tenha sido definido 
    anteriormente */
#ifndef MACRO_FILA_FIXA_CODIGO
#define MACRO_FILA_FIXA_CODIGO 12
#endif /* MACRO_FILA_FIXA_CODIGO */

/* Esse código é usado nas diretrizes de pré-processador para dinamicamente
   adaptar o código da pilha linear fixa para o tipo de dado selecionado (Em C 
   não conseguimos fazer comparação de textos em macros, somente números) a 
   baixo você encontra a tabela com os códigos para cada tipo:
        0 - signed char   4 - unsigned char   8  - char  12 - float
        1 - signed short  5 - unsigned short  9  - short 13 - double
        2 - signed int    6 - unsigned int    10 - int
        3 - signed long   7 - unsigned long   11 - long
        
    o #ifndef é uma medida de segurança para impedir que 
    MACRO_PILHA_FIXA_CODIGO seja re-definido caso tenha sido definido 
    anteriormente */
#ifndef MACRO_PILHA_FIXA_CODIGO
#define MACRO_PILHA_FIXA_CODIGO 12
#endif /* MACRO_PILHA_FIXA_CODIGO */

#endif /* CONFIG_H */
