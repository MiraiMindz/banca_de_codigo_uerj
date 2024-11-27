/*******************************************************************************
 * Esse é um arquivo de cabeçalho em C, ele é usado para definir e exportar
 * funções e macros para outros arquivos.
 *
*******************************************************************************/

#ifndef ESTRUTURAS_FILA_ESTATICA_LINEAR_ESTRUTURA
#define ESTRUTURAS_FILA_ESTATICA_LINEAR_ESTRUTURA

#include "macros.h"

/* Uma struct que representa uma fila estatica, a ordem dos campos tem ligeira
   importancia, pois permite com que otimizemos os espaços em memoria sem usar
   padding, o que agiliza o acesso e a performance da struct. Além de otimizar
   o acesso aos campos, pois mantemos os campos de maior acesso (inicio e fim)
   no inicio da struct, enquanto mantemos os campos menos acessados no final
   o layout mais comum é ordenação por tamanho, onde os menores tipos estão no
   topo e os maiores no final, como o tipo dos dados é um ponteiro geralmente
   definimos no final pelo fato do tamanho do ponteiro poder variar. */
typedef struct fila_estatica_s {
    /* representa o indice do ultimo elemento na fila, é usado para remover um
       elemento da fila e também para saber se a fila está cheia ou vazia. */
    unsigned long inicio;

    /* representa o indice do ultimo elemento na fila, é usado para adicionar um
       elemento da fila e também para saber se a fila está cheia ou vazia. */
    unsigned long final;

    /* representa a capacidade total da fila, é usado para alocarmos previamente
       a memória da fila usando calloc(). */
    unsigned long capacidade;

    /* Ponteiro para os dados, pode ser um único número ou uma array.
       poderiamos ter utilizado uma array alocada na heap, e não uma alocação na
       stack por exemplo:
        int[255] -> alocação na heap
        int* = malloc(sizeof(int) * 255) -> alocação na stack
       poderiamos ter usado a heap justamente pelo fato da fila ser estatica
       pois a heap não permite alocação e realocação dinamica de memória
       somente a stack permite, mas como o intuito é usar malloc/calloc,
       eu usei a stack para guardar os dados.

       embora a alocação seja dinamica o tipo de dado não é. */
    MACRO_FILA_ESTATICA_TIPO *dados_ptr;
} fila_estatica_t;

/* Prototipos de funções, o objetivo de um prototipo de função é definir
   previamente a declaração para que o compilador possa fazer checagem de tipos
   e localize a função, permitindo por exemplo, chamarmos no main() uma função
   definida posteriormente, basta que seu prototipo seja anterior a main().

   Um prototipo de função é uma declaração que define o tipo de retorno, nome e
   parametros de uma função (esse conjunto também é conhecido como "assinatura")
 */

/* essa função pega uma dada capacidade e cria uma fila estatica com tal tamanho
 */
fila_estatica_t *criar_fila_estatica_ret(unsigned long capacidade);

/* Essa função "destroi" a fila, liberando toda a memoria alocada para a mesma
 */
void destruir_fila_estatica_ptr(fila_estatica_t *fila_ptr);

/* essa função insere um valor em uma dada fila estatica e retorna um código de
   erro onde 0 significa sucesso. */
int inserir_valor_fila_estatica_ret(fila_estatica_t *fila_ptr,
                                    MACRO_FILA_ESTATICA_TIPO valor);

/* essa função remove e retorna um valor em uma dada fila estatica */
MACRO_FILA_ESTATICA_TIPO
remover_valor_fila_estatica_ret(fila_estatica_t *fila_ptr);

/* essa função reinicia a fila, limpando todos os valores e re-definindo os
   indices para zero */
void reiniciar_fila_estatica(fila_estatica_t *fila_ptr);

/* essa função move e re-aloca os elementos da lista estatica */
void realocar_mover_fila_estatica(fila_estatica_t *fila_ptr);

/* essa função insere um valor em uma posição especifica da lista */
int inserir_valor_posicao_fila_estatica(fila_estatica_t *fila_ptr,
                                        unsigned long indice,
                                        MACRO_FILA_ESTATICA_TIPO valor);

/* essa função remove um valor em uma posição especifica da lista. */
int remover_valor_posicao_fila_estatica(fila_estatica_t *fila_ptr,
                                        unsigned long indice);

#endif /* ESTRUTURAS_FILA_ESTATICA_LINEAR_ESTRUTURA */
