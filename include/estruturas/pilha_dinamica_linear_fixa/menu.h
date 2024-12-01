/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
*******************************************************************************/

#ifndef ESTRUTURAS_PILHA_FIXA_LINEAR_MENU
#define ESTRUTURAS_PILHA_FIXA_LINEAR_MENU

#include <estruturas/pilha_dinamica_linear_fixa/estrutura.h>

/* essa função exibe a pilha fixa, o parametro de modificadores altera o modo
   de exibição da pilha, ele utiliza mascara de bits para selecionar os valores.
   modificadores:
    - (0b10000000 - 1 bit) exibir conectores: exibe uma seta "->" entre os
        valores caso o bit seja zero, irá exibir uma virgula no lugar.
    - (0b01000000 - 2 bit) exibir endereços base, atual e topo: exibe os
        endereços da base e do topo sempre e exibe o endereço do valor
        adicionado ou removido da pilha quando executado.
    - (0b00100000 - 3 bit) exibir como tabela: exibe a pilha em forma de tabela
        incompativel com o modificador do bit 1.
    - (0b00010000 - 4 bit) exibir todos os endereços: modificador da forma de
        tabelas, exibe os endereços na memória de todos os valores.
    - (0b00001000 - 5 bit) exibir todas as posições: modificador da forma de
        tabelas, exibe as posições (indices) dos elementos, também podemos dizer
        que ela exibe a ordem de remoção dos valores.
    - (0b00010000 - 6 bit) exibir pilha completa: exibe todos os valores
        inclusive aqueles que foram removidos, e não somente os valores
        significativos da pilha.
    - (bits 7 a 8): reservado.
   como você define esses modificadores?
    usando o operador OR (|), exemplo:
        (1 << 0) | (1 << 3) | (1 << 5)
        define os modficadores 1, 4 e 6, o indice dos modificadores começa em 0.
    definindo somente 0 usará o padrão (pilha com separador de virgula).
   exemplos de uso:
        exibir_pilha_fixa(pilha_ptr, (1 << 0) | (1 << 3) | (1 << 5));
        exibir_pilha_fixa(pilha_ptr, 0);

   Porque usar mascara de bits? Para uma lista de booleanos é mais eficiente
   usar a representação binária de um numero do que uma array de booleanos. Você
   pode ver mais nesse programa <https://onlinegdb.com/m0GW2gh22> Tamanho do
   booleano padrão: 1 byte(s) (8 bits) Tamanho do booleano customizado: 4
   byte(s) (32 bits) Tamanho de um char: 1  byte(s) (8 bits) Tamanho da lista de
   8 booleanos padrões: 8 byte(s) (64 bits) Tamanho da lista de 8 booleanos
   customizados: 32 byte(s) (256 bits) Usando um char ao invés de um booleano
   padrão temos uma economia de 87.5% para armazenar a mesma quantidade de
   informação. Usando um char ao invés de um booleano customizado temos uma
   economia de 96.875% para armazenar a mesma quantidade de informação. */
void exibir_pilha_fixa(pilha_linear_fixa_t *pilha_ptr,
                          unsigned char modificadores);

/* essa função testa a pilha fixa de forma interativa com um menu. */
void testar_pilha_fixa_menu_interativo(void);

#endif /* ESTRUTURAS_PILHA_FIXA_LINEAR_MENU */
