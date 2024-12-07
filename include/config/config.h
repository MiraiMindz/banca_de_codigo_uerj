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

/* Esse macro permite o uso de cores no terminal, colorizando todas as saidas.
 * 1 significa SIM, 0 significa NÃO; qualquer valor diferente pode ter 
 * comportamento inesperado */
#ifndef _USAR_COR_NO_TERMINAL
#define _USAR_COR_NO_TERMINAL 1
#endif

/* Os dois macros controlam a exibição da função less para exibição de páginas
 * de texto dinamicas, por favor lembre-se de usar um numero para a altura
 * MENOR do que a quantidades de linha no seu terminal, você pode ver essa
 * informação usando os seguintes comandos:
 * CMD:
 *      mode
 * PowerShell:
 *      "Linhas: $($Host.UI.RawUI.WindowSize.Height)"
 *      "Colunas: $($Host.UI.RawUI.WindowSize.Width)"
 * Linux (qualquer shell POSIX):
 *      tput cols lines
 * */
#ifndef _CLI_LESS_ALTURA_DA_PAGINA
#define _CLI_LESS_ALTURA_DA_PAGINA 24  /* numero de linhas na tela */
#endif

#ifndef _CLI_LESS_LARGURA_DA_PAGINA
#define _CLI_LESS_LARGURA_DA_PAGINA 80    /* largura da linha */
#endif

/* Esses dois macros controlam a largura e a altura da interface gráfica,
 * Ajuste para adequar ao seu monitor, a resolução padrão é 960x720 (4:3)
 * para funcionar em projetores, porém a janela é re-ajustavél. */
#ifndef _INTERFACE_GRAFICA_LARGURA
#define _INTERFACE_GRAFICA_LARGURA 960
#endif

#ifndef _INTERFACE_GRAFICA_ALTURA
#define _INTERFACE_GRAFICA_ALTURA 720
#endif

/* Use multiplos de 8, o tamanho base da fonte é 16 então estamos escalando a
 * fonte 1.5x para a resolução de 960x720, abaixo disso por favor usar 16 ou
 * menos, e acima disso favor escalar a fonte proporcionalemente */
#ifndef _INTERFACE_GRAFICA_TAMANHO_FONTE
#define _INTERFACE_GRAFICA_TAMANHO_FONTE 24
#endif

/* Esse macro controla a taxa de quadros por segundo da interface grafica, o
 * padrão é 30, mas sinta-se a vontade para definir para 60 ou valores maiores
 * dependendo do seu monitor */
#ifndef _INTERFACE_GRAFICA_QUADROS_POR_SEGUNDO
#define _INTERFACE_GRAFICA_QUADROS_POR_SEGUNDO 30
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
