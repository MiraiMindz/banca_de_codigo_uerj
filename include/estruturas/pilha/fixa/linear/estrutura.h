/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
 *******************************************************************************
 *                               PILHA LINEAR FIXA
 *******************************************************************************
 * Uma Pilha fixa é um elemento LIFO (Last In First Out) ou UEPS (Ultimo a
 * Entrar Primeiro a Sair), que não altera seu tamanho de forma dinamica podendo
 * somente armazernar um numero fixo de items.
 * 
 * uma pilha como o nome sugere é o equivalente a uma pilha "de algo" no mundo
 * real, por exemplo uma pilha de pratos, o primeiro elemento a entrar fica no
 * "fundo" da pilha, sendo o ultimo a sair, portanto o ultimo elemento fica no
 * topo da pilha, sendo o primero a sair.
 * 
 * ela é dinamica no sentido de sua alocação de memória ser feita na STACK ao
 * invés da HEAP, sendo "dinamicamente alocada", porém ela tem TAMANHO FIXO
 * ou seja, ela não aumenta para suportar mais itens que o previamente estabele-
 * cido.
*******************************************************************************/

#ifndef ESTRUTURAS_PILHA_FIXA_LINEAR_ESTRUTURA
#define ESTRUTURAS_PILHA_FIXA_LINEAR_ESTRUTURA

#include <estruturas/pilha/fixa/linear/macros.h>

/* Uma struct que representa uma pilha linear de alocação dinamica e tamanho fixo
 * a ordem dos campos tem ligeira importancia, pois permite com que otimizemos 
 * os espaços em memoria sem usar padding, o que agiliza o acesso e a 
 * performance da struct. Além de otimizar o acesso aos campos, pois mantemos os
 * campos de maior acesso (capacidade e topo) no inicio da struct, enquanto  
 * mantemos os campos menos acessados no final o layout mais comum é ordenação 
 * por tamanho, onde os menores tipos estão no topo e os maiores no final, como  
 * o tipo dos dados é um ponteiro geralmente definimos no final pelo fato do 
 * tamanho do ponteiro poder variar.
**/
typedef struct pilha_fixa_linear_s {
    /* Representa a capacidade total da pilha, é usada para sabermos se ela está
    cheia, ou vazia e também para alocarmos previamente a memoria da fila usando
    calloc() */
    unsigned long capacidade;

    /* Representa o indice do topo/ultimo elemento da pilha, é necessário para
    sabermos se a pilha está cheia e para adicionarmos um novo elemento ao topo
    */
    unsigned long topo;
    
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
    MACRO_PILHA_FIXA_LINEAR_TIPO* dados_ptr;
} pilha_fixa_linear_t;

/* Prototipos de funções, o objetivo de um prototipo de função é definir
 * previamente a declaração para que o compilador possa fazer checagem de tipos
 * e localize a função, permitindo por exemplo, chamarmos no main() uma função
 * definida posteriormente, basta que seu prototipo seja anterior a main().
 * 
 * Um prototipo de função é uma declaração que define o tipo de retorno, nome e
 * parametros de uma função (esse conjunto também é conhecido como "assinatura")
 * 
 * prototipos são mais usadados em arquivos de cabeçalho mas também podemos
 * defini-los em arquivos de código fonte .c
**/

/* Cria e retorna uma pilha linear como o tamanho fixo definido */
pilha_fixa_linear_t *criar_pilha_fixa_linear_ret(unsigned long capacidade);

/* Insere um valor no topo da pilha, retorna um inteiro representando o estado
 * da operação onde 0 significa sucesso e qualquer número diferente erro. */
int inserir_pilha_fixa_linear_ret(pilha_fixa_linear_t* pilha_ptr, MACRO_PILHA_FIXA_LINEAR_TIPO valor);

/* Retira e retorna um um valor do topo da pilha */
MACRO_PILHA_FIXA_LINEAR_TIPO retirar_pilha_fixa_linear_ret(pilha_fixa_linear_t* pilha_ptr);

/* "Retorna" o valor no topo da pilha sem remove-lo, é util quando queremos 
 * "observar" o topo da pilha, para por exemplo, checar se um valor foi
 * adicionado previamente e não repeti-lo */
MACRO_PILHA_FIXA_LINEAR_TIPO observar_topo_pilha_fixa_linear_ret(pilha_fixa_linear_t* pilha_ptr);

/* essa função reinicia a pilha, limpando todos os valores e re-definindo os
 * indices para zero */
void reiniciar_pilha_fixa_linear(pilha_fixa_linear_t* pilha_ptr);

/* Essa função destroi a pilha, liberando toda a memoria alocada para a mesma */
void limpar_pilha_fixa_linear(pilha_fixa_linear_t* pilha_ptr);

#endif /* ESTRUTURAS_PILHA_FIXA_LINEAR_ESTRUTURA */
