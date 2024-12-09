/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include <estruturas/fila/fixa/linear/estrutura.h>
#include <stdlib.h>

fila_fixa_linear_t *criar_fila_fixa_linear_ret(unsigned long capacidade) {
    /* Usamos typedef na implementação do tipo para podermos usar malloc sem o
       especificador de struct: malloc(sizeof(struct fila_fixa_s)); */
    fila_fixa_linear_t *fila;
    fila = malloc(sizeof(fila_fixa_linear_t));
    if (!fila) {
        return NULL; /* retorna NULO se a alocação falhou */
    }

    /* usamos o macro MACRO_FILA_FIXA_LINEAR_TIPO para alocar o espaço necessário
       para a quantidade de dados necessários, e fazemos type-casting
       para o tipo esperado. */
    fila->dados_ptr = (MACRO_FILA_FIXA_LINEAR_TIPO*)calloc(capacidade, 
        sizeof(MACRO_FILA_FIXA_LINEAR_TIPO));
    if (!fila->dados_ptr) {
        free(fila);  /* Libera a memória já alocada se falhar */
        return NULL; /* retorna NULO se a alocação falhou */
    }

    /* a fila estará vazia então o inicio é 0. */
    fila->inicio = 0;

    /* a fila estará vazia então o final é 0. */
    fila->final = 0;

    /* guardaremos a capacidade da fila na struct para podermos evitar um
     * overflow */
    fila->capacidade = capacidade;
    return fila;
}

void destruir_fila_fixa_linear_ptr(fila_fixa_linear_t *fila_ptr) {
    free(fila_ptr->dados_ptr);
    free(fila_ptr);
}

int inserir_valor_fila_fixa_linear_ret(fila_fixa_linear_t *fila_ptr,
                                    MACRO_FILA_FIXA_LINEAR_TIPO valor) {
    if (fila_ptr->final >= fila_ptr->capacidade) {
        return -1; /* A fila está cheia */
    }

    /* Associamos o valor desejado na ultima posição */
    fila_ptr->dados_ptr[fila_ptr->final] = valor;

    /* incrementamos o indice de final da fila */
    fila_ptr->final++;
    return 0;
}

MACRO_FILA_FIXA_LINEAR_TIPO
remover_valor_fila_fixa_linear_ret(fila_fixa_linear_t *fila_ptr) {
    int valor;
    if (fila_ptr->inicio == fila_ptr->final) {
        return -1; /* Fila Cheia */
    }

    /* coletamos o valor */
    valor = fila_ptr->dados_ptr[fila_ptr->inicio];

    /* "limpamos" a celula de memória. */
    fila_ptr->dados_ptr[fila_ptr->inicio] = 0;

    /* Incrementamos o inicio. */
    fila_ptr->inicio = fila_ptr->inicio + 1;
    return valor;
}

void reiniciar_fila_fixa_linear(fila_fixa_linear_t *fila_ptr) {
    unsigned long elementos_presentes;
    unsigned long i;

    /* Calcula o total de items presentes na fila */
    elementos_presentes = fila_ptr->final - fila_ptr->inicio;

    /* Navega pela fila zerando todos os elementos */
    for (i = 0; i < elementos_presentes; i++) {
        fila_ptr->dados_ptr[i] = 0;
    }

    /* Zera os indices de inicio e final */
    fila_ptr->inicio = 0;
    fila_ptr->final = 0;
}

void realocar_mover_fila_fixa_linear(fila_fixa_linear_t *fila_ptr) {
    /* Calcula o número de elementos presentes na fila */
    unsigned long elementos_presentes;
    unsigned long i;
    elementos_presentes = fila_ptr->final - fila_ptr->inicio;
    ;

    /* Move os elementos do índice 'inicio' para o começo do array */
    for (i = 0; i < elementos_presentes; i++) {
        fila_ptr->dados_ptr[i] = fila_ptr->dados_ptr[fila_ptr->inicio + i];
    }

    /* Ajusta os índices 'inicio' e 'final' para refletir a nova posição
       dos elementos */
    fila_ptr->inicio = 0;
    fila_ptr->final = elementos_presentes;

    /* Reinicia os valores a partir do último valor até a capacidade final para
     * 0 */
    for (i = fila_ptr->final; i < fila_ptr->capacidade; i++) {
        fila_ptr->dados_ptr[i] = 0; /* Define o valor como 0 */
    }
}
