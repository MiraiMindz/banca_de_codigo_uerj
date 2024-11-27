#include "estruturas/fila_estatica_linear/estrutura.h"
#include <stdlib.h>

fila_estatica_t *criar_fila_estatica_ret(unsigned long capacidade) {
    /* Usamos typedef na implementação do tipo para podermos usar malloc sem o
       especificador de struct: malloc(sizeof(struct fila_estatica_s)); */
    fila_estatica_t *fila;
    fila = malloc(sizeof(fila_estatica_t));
    if (!fila) {
        return NULL; /* Verifica se a alocação falhou */
    }

    /* usamos o macro MACRO_FILA_ESTATICA_TIPO para alocar o espaço necessário
       para a quantidade de dados necessários. */
    fila->dados_ptr = calloc(capacidade, sizeof(MACRO_FILA_ESTATICA_TIPO));
    if (!fila->dados_ptr) {
        free(fila);  /* Libera a memória já alocada se falhar */
        return NULL; /* Verifica se a alocação falhou */
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

void destruir_fila_estatica_ptr(fila_estatica_t *fila_ptr) {
    free(fila_ptr->dados_ptr);
    free(fila_ptr);
}

int inserir_valor_fila_estatica_ret(fila_estatica_t *fila_ptr,
                                    MACRO_FILA_ESTATICA_TIPO valor) {
    if (fila_ptr->final >= fila_ptr->capacidade) {
        return -1; /* A fila está cheia */
    }

    /* Associamos o valor desejado na ultima posição */
    fila_ptr->dados_ptr[fila_ptr->final] = valor;

    /* incrementamos o indice de final da fila */
    fila_ptr->final++;
    return 0;
}

MACRO_FILA_ESTATICA_TIPO
remover_valor_fila_estatica_ret(fila_estatica_t *fila_ptr) {
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

void reiniciar_fila_estatica(fila_estatica_t *fila_ptr) {
    unsigned long elementos_presentes;
    unsigned long i;

    /* Calcula o total de items presentes na lista */
    elementos_presentes = fila_ptr->final - fila_ptr->inicio;

    /* Navega pela lista zerando todos os elementos */
    for (i = 0; i < elementos_presentes; i++) {
        fila_ptr->dados_ptr[i] = 0;
    }

    /* Zera os indices de inicio e final */
    fila_ptr->inicio = 0;
    fila_ptr->final = 0;
}

void realocar_mover_fila_estatica(fila_estatica_t *fila_ptr) {
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

int inserir_valor_posicao_fila_estatica(fila_estatica_t *fila_ptr,
                                        unsigned long indice,
                                        MACRO_FILA_ESTATICA_TIPO valor) {
    unsigned long i;
    if (fila_ptr->final == fila_ptr->capacidade) {
        return -1; /* A fila está cheia */
    }

    if (indice >= fila_ptr->capacidade) {
        return -2; /* Índice fora do intervalo da capacidade total */
    }

    /* Desloca elementos à direita a partir do índice desejado */
    for (i = fila_ptr->final; i > indice; i--) {
        fila_ptr->dados_ptr[i] = fila_ptr->dados_ptr[i - 1];
    }

    /* Insere o valor na posição desejada */
    fila_ptr->dados_ptr[indice] = valor;

    /* Atualiza 'final' caso o valor seja inserido em uma posição nova (após o
     * atual 'final') */
    if (indice >= fila_ptr->final) {
        fila_ptr->final = indice + 1;
    } else {
        fila_ptr->final++;
    }

    return 0; /* Sucesso */
}

int remover_valor_posicao_fila_estatica(fila_estatica_t *fila_ptr,
                                        unsigned long indice) {
    unsigned long i;
    /* Verifica se a fila não está vazia */
    if (fila_ptr->inicio == fila_ptr->final) {
        return -1; /* A fila está vazia */
    }

    /* Verifica se o índice está dentro do intervalo */
    if (indice < fila_ptr->inicio || indice >= fila_ptr->final) {
        return -2; /* Índice fora do intervalo */
    }

    /* Desloca elementos à esquerda a partir do índice desejado */
    for (i = indice; i < fila_ptr->final - 1; i++) {
        fila_ptr->dados_ptr[i] = fila_ptr->dados_ptr[i + 1];
    }

    /* Zera o último valor (agora duplicado) */
    fila_ptr->dados_ptr[fila_ptr->final - 1] =
        0; /* Ou qualquer valor que represente "vazio" */

    /* Atualiza 'final' */
    fila_ptr->final--;

    return 0; /* Sucesso */
}
