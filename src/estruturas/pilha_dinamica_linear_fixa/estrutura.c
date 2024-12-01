/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include <estruturas/pilha_dinamica_linear_fixa/estrutura.h>
#include <stdlib.h>
#include <string.h>

/* Cria e retorna uma pilha linear como o tamanho fixo definido */
pilha_linear_fixa_t *criar_pilha_fixa_ret(unsigned long capacidade) {
    /* Usamos typedef na implementação do tipo para podermos usar malloc sem o
       especificador de struct: malloc(sizeof(struct pilha_linear_fixa_s)); */
    pilha_linear_fixa_t *pilha_ptr;
    pilha_ptr = malloc(sizeof(pilha_linear_fixa_t));
    if (!pilha_ptr) {
        return NULL; /* retorna NULO se a alocação falhou */
    }

    /* usamos o macro MACRO_PILHA_FIXA_TIPO para alocar o espaço necessário
       para a quantidade de dados necessários, e fazemos type-casting
       para o tipo esperado. */
    pilha_ptr->dados_ptr = (MACRO_PILHA_FIXA_TIPO*)calloc(capacidade, 
        sizeof(MACRO_PILHA_FIXA_TIPO));
    if (!pilha_ptr->dados_ptr) {
        free(pilha_ptr); /* Libera a memória já alocada se falhar */
        return NULL; /* retorna NULO se a alocação falhou */
    }

    pilha_ptr->topo = 0; /* a pilha está vazia/existem 0 elementos na pilha */
    /* normalmente definimos o topo como -1 pois usamos ints, mas como nessa
    implementação usamos unsigned long (um numero longo sem sinal), não temos
    números negativos então o valor sentinela passou a ser 0, o que coincide com
    a forma humana de interpretar as coisas facilitando a compreensão. */
    pilha_ptr->capacidade = capacidade;
    return pilha_ptr;
}

/* Insere um valor no topo da pilha, retorna um inteiro representando o estado
 * da operação onde 0 significa sucesso e qualquer número diferente erro. */
int inserir_pilha_fixa(pilha_linear_fixa_t* pilha_ptr, MACRO_PILHA_FIXA_TIPO valor) {
    if (pilha_ptr->topo == pilha_ptr->capacidade) {
        return -1; /* a pilha está cheia e portanto não podemos adicionar. */
    }

    /* Aqui nós acessamos o valor na array que está no indice de topo, e depois
    incrementamos o topo, como o topo é indexado por 1, ele sempre irá apontar
    para o próximo indice na lista. */
    pilha_ptr->dados_ptr[pilha_ptr->topo++] = valor;
    return 0;
}

/* Retira e retorna um um valor do topo da pilha */
MACRO_PILHA_FIXA_TIPO retirar_pilha_fixa(pilha_linear_fixa_t* pilha_ptr) {
    MACRO_PILHA_FIXA_TIPO valor;

    if (pilha_ptr->topo == 0) {
        return -1; /* a pilha está vazia e portanto não podemos remover. */
    }

    /* Aqui nós decrementamos o topo primeiro antes de acessar seu valor, pois
    o topo é indexado a 1 e a pilha é indexada a 0 então o ultimo valor da pilha
    será sempre topo - 1, fazendo isso nós garantimos que o topo sempre acessará
    o valor correto na pilha */
    valor = pilha_ptr->dados_ptr[--pilha_ptr->topo];
    /* Aqui nós "limpamos" a célula de memória */
    pilha_ptr->dados_ptr[pilha_ptr->topo] = 0;
    return valor;
}

/* "Retorna" o valor no topo da pilha sem remove-lo, é util quando queremos 
 * "observar" o topo da pilha, para por exemplo, checar se um valor foi
 * adicionado previamente e não repeti-lo */
MACRO_PILHA_FIXA_TIPO observar_topo_pilha_fixa(pilha_linear_fixa_t* pilha_ptr) {
    if (pilha_ptr->topo == 0) {
        return -1; /* a pilha está vazia e portanto não podemos remover. */
    }

    /* Como o topo representa o proximo indice na pilha retornamos topo - 1 */
    return pilha_ptr->dados_ptr[pilha_ptr->topo - 1];
}

/* essa função reinicia a pilha, limpando todos os valores e re-definindo os
 * indices para zero */
void reiniciar_pilha_fixa(pilha_linear_fixa_t* pilha_ptr) {
    /* definimos novamente o valor do topo para nosso valor sentinela */
    pilha_ptr->topo = 0;

    /* usamos memset para reiniciar todos os valores da pilha para 0 */
    memset(pilha_ptr->dados_ptr, 0, sizeof(MACRO_PILHA_FIXA_TIPO) * pilha_ptr->capacidade);
}

/* Essa função destroi a pilha, liberando toda a memoria alocada para a mesma */
void limpar_pilha_fixa(pilha_linear_fixa_t* pilha_ptr) {
    free(pilha_ptr->dados_ptr);
    free(pilha_ptr);
}
