/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
 *******************************************************************************
 *                               FILA LINEAR FIXA
 *******************************************************************************
 * Uma Fila fixa é um elemento FIFO (First In First Out) ou PEPS (Primeiro a
 * Entrar Primeiro a Sair), que não altera seu tamanho de forma dinamica podendo
 * somente armazernar um numero fixo de items.
 * 
 * Uma fila como o nome sugere é o equivalente a uma fila no mundo real com
 * inicio, fim e um tamanho, onde o primeiro a chegar é o primeiro a sair.
 * 
 * ela é dinamica no sentido de sua alocação de memória ser feita na STACK ao
 * invés da HEAP, sendo "dinamicamente alocada", porém ela tem TAMANHO FIXO
 * ou seja, ela não aumenta para suportar mais itens que o previamente estabele-
 * cido.
*******************************************************************************/

#ifndef ESTRUTURAS_FILA_DINAMICA_LINEAR_FIXA_ESTRUTURA
#define ESTRUTURAS_FILA_DINAMICA_LINEAR_FIXA_ESTRUTURA

#include "macros.h"

/* Uma struct que representa uma fila linear de alocação dinamica e tamanho fixo
   a ordem dos campos tem ligeira importancia, pois permite com que otimizemos 
   os espaços em memoria sem usar padding, o que agiliza o acesso e a 
   performance da struct. Além de otimizar o acesso aos campos, pois mantemos os
   campos de maior acesso (inicio e fim) no inicio da struct, enquanto mantemos 
   os campos menos acessados no final o layout mais comum é ordenação por 
   tamanho, onde os menores tipos estão no topo e os maiores no final, como o 
   tipo dos dados é um ponteiro geralmente definimos no final pelo fato do 
   tamanho do ponteiro poder variar. */
typedef struct fila_linear_fixa_s {
    /* representa o indice do ultimo elemento na fila, é usado para remover um
       elemento da fila e também para saber se a fila está cheia ou vazia. */
    unsigned long inicio;

    /* representa o indice do ultimo elemento na fila, é usado para adicionar um
       elemento da fila e também para saber se a fila está cheia ou vazia. */
    unsigned long final;

    /* representa a capacidade total da fila, é usado para alocarmos previamente
       a memória da fila usando calloc(), também usamos para saber se a fila
       está cheia ou vazia. */
    unsigned long capacidade;

    /* Ponteiro para os dados, pode ser um único número ou uma array.
       poderiamos ter utilizado uma array alocada na heap, e não uma alocação na
       stack por exemplo:
        int[255] -> alocação na heap
        int* = malloc(sizeof(int) * 255) -> alocação na stack
       poderiamos ter usado a heap justamente pelo fato do tamanho da fila ser
       fixo pois a heap não permite alocação e realocação dinamica de memória
       somente a stack permite, mas como o intuito é usar malloc/calloc,
       eu usei a stack para guardar os dados.

       embora a alocação seja dinamica o tipo de dado não é. */
    MACRO_FILA_FIXA_TIPO *dados_ptr;
} fila_linear_fixa_t;

/* Prototipos de funções, o objetivo de um prototipo de função é definir
   previamente a declaração para que o compilador possa fazer checagem de tipos
   e localize a função, permitindo por exemplo, chamarmos no main() uma função
   definida posteriormente, basta que seu prototipo seja anterior a main().

   Um prototipo de função é uma declaração que define o tipo de retorno, nome e
   parametros de uma função (esse conjunto também é conhecido como "assinatura")

   prototipos são mais usadados em arquivos de cabeçalho mas também podemos
   defini-los em arquivos de código fonte .c.
*/

/* essa função pega uma dada capacidade e cria uma fila fixa com tal tamanho
 */
fila_linear_fixa_t *criar_fila_fixa_ret(unsigned long capacidade);

/* Essa função destroi a fila, liberando toda a memoria alocada para a mesma */
void destruir_fila_fixa_ptr(fila_linear_fixa_t *fila_ptr);

/* essa função insere um valor em uma dada fila fixa e retorna um código de
   erro onde 0 significa sucesso e qualquer número diferente erro. */
int inserir_valor_fila_fixa_ret(fila_linear_fixa_t *fila_ptr,
                                    MACRO_FILA_FIXA_TIPO valor);

/* essa função remove e retorna um valor em uma dada fila fixa */
MACRO_FILA_FIXA_TIPO
remover_valor_fila_fixa_ret(fila_linear_fixa_t *fila_ptr);

/* essa função reinicia a fila, limpando todos os valores e re-definindo os
   indices para zero */
void reiniciar_fila_fixa(fila_linear_fixa_t *fila_ptr);

/* essa função move e re-aloca os elementos da fila fixa */
void realocar_mover_fila_fixa(fila_linear_fixa_t *fila_ptr);

#endif /* ESTRUTURAS_FILA_FIXA_LINEAR_ESTRUTURA */
