/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
 *
 * Esse arquivo em específico é o arquivo de configuração do codigo, ele define
 * os tipos de dado, e o comportamento do código em geral.
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

/* Esse código é usado nas diretrizes de pré-processador para dinamicamente
   adaptar o código para o tipo de dado selecionado (Em C não conseguimos fazer
   comparação de textos em macros, somente números) a baixo você encontra a
   tabela com os códigos para cada tipo:
        0 - signed char   4 - unsigned char   8  - char  12 - float
        1 - signed short  5 - unsigned short  9  - short 13 - double
        2 - signed int    6 - unsigned int    10 - int
        3 - signed long   7 - unsigned long   11 - long
        
    o #ifndef é uma medida de segurança para impedir que 
    MACRO_FILA_ESTATICA_CODIGO seja re-definido caso tenha sido definido 
    anteriormente */
#ifndef MACRO_FILA_ESTATICA_CODIGO
#define MACRO_FILA_ESTATICA_CODIGO 12
#endif /* MACRO_FILA_ESTATICA_CODIGO */

#endif /* CONFIG_H */
