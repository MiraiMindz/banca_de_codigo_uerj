/*******************************************************************************
 *
 * LEIA-ME   LEIA-ME   LEIA-ME   LEIA-ME   LEIA-ME   LEIA-ME   LEIA-ME   LEIA-ME
 *
 *******************************************************************************
 * Esse pequeno arquivo usa o padrão ISO/ANSI C89. O primeiro padrão oficial da
 * linguagem lançado em 1989 pela Organização Internacional de Padronizações.
 *
 * Este arquivo contém extensos comentários explicando cada implementação e modo
 * de utilização, para uma melhor compreensão do mesmo.
 *
 * ele será dividido por estrutura de dado e não por categoria, ou seja ao invés
 * de seguir a seguinte estrutura:
 *  1. Definições de tipos
 *  2. Prototipo de funções
 *  3. Implementação de funções
 *  4. Testes e modo de uso.
 * Ele ira usar esta estrutura:
 *  1. Lista Encadeada <Estrutura de Dado Implementada>
 *      1.1. Definições de tipos
 *      1.2. Prototipo de funções
 *      1.3. Implementação de funções
 *      1.4. Testes e modo de uso.
 *  2. Arvore <Estrutura de Dado Implementada>
 *      2.1. Definições de tipos
 *      2.2. Prototipo de funções
 *      2.3. Implementação de funções
 *      2.4. Testes e modo de uso.
 * e assim sucessivamente, para manter a estrategia de LOC (Locality of
 *Concerns) e também para servir como um pequeno prototipo alfa para a
 *biblioteca de código da instituição.
 *
 * Tipos implementados:
 *  - Fila (Lista)
 *      - Versões: Alocação Estatica e Alocação Dinâmica.
 *      - Linear e Circular.
 *      - Simples, Encadeada e Duplamente Encadeada.
 *  - Pilha (Stack)
 *      - Versões: Alocação Estatica e Alocação Dinâmica.
 *      - Linear e Circular.
 *  - Arvore
 *  - Tabela Hash
 *
 * Os nomes das variaveis também estarão em português usando o seguinte esquema
 * de nomeclatura:
 *  - Convenção de Nomeclatura: snake_case
 *  - Sufixos:
 *      - _s   -> Structs
 *      - _t   -> Tipos
 *      - _g   -> Variaveis Globais
 *      - _ptr -> Ponteiros
 *      - _ret -> Retorno
 *  - Prefixos:
 *      - MACRO: Representa uma diretriz de pré-processador de macro.
 * os sufixos _ptr e _ret serão mais utilizados em funções para representar
 * funções que usam pointeros para retornar seu tipo ou se criam uma nova
 * variavel através da palavra-chave return, porém o sufixo _ptr também será
 * usado em variaveis do tipo ponteiro.
 *
 * A respeito dos tipos, eu irei explicitamente definir tipos com sinal ou não.
 *
 * além de não usar o tipo long long para definir numeros de ao menos 64 bits,
 * diferente de apenas long, que pode ser tanto 32 quanto 64, long long é
 * garantido de ter ao menos 64 bits, porém a ideia é ser ESTRITAMENTE C89, e o
 * suporte para long long foi definido apenas no C99, embora alguns compiladores
 * como o GCC e LLVM-CLANG suportam long long como uma extensão em C89, eu irei
 * evitar o uso por não ser garantido compatibilidade de execução, então tenha
 * em mente que o valor maximo de bits para um inteiro será 32 bits:
 *  -   valores entre -2147483648 e 2147483647 ou 0 e 4294967295.
 *
 * ANSI C89 também não tem os tipos size_t e ssize_t, esses tipos foram
 * definidos no padrão C90, então usarei unsigned e signed long para isso.
 *
 * Também usarei o tipo char como um numero de 8 bits e não como um "caractere"
 * (tecnicamente falando todo caractere é uma representação de algum valor
 * 8 bits) mas esclareço isto para o leitor não estranhar o uso de char em
 * contextos numéricos (como loops).
 *
 * Caso queira veja a tabela dos tipos que eu irei usar e seus significados:
 *
 *  - float: numero de ponto flutuante com sinal e 7/8 casas de precisão.
 *  - double: numero de ponto flutuante com sinal e 15/16 casas de precisão.
 *
 *  - unsigned char: numero inteiro sem sinal de 8 bits.
 *      - Valores entre: 0 e 255.
 *
 *  - unsigned short: numero inteiro sem sinal de 16 bits.
 *      - Valores entre: 0 e 65535.
 *
 *  - unsigned int: numero inteiro sem sinal de 16 ou 32 bits.
 *      - O int tem 16 ou 32 bits dependendo do sistema ou plataforma.
 *      - Valores entre: 0 e  65535 em 16 bits.
 *      - Valores entre: 0 e  4294967295 em 32 bits.
 *
 *  - unsigned long: numero inteiro sem sinal maior que 16 bits.
 *      - o padrão ANSI C89 define que long deve ao menos ser maior que int ou
 *      - ter o mesmo tamanho, então ele pode ter 29 bits, 64 bits, 12000 bits
 *      - não é garantido um tamanho especifico para ele, nesse codigo ele será
 *      - usado no contexto de "um número com 16 bits ou mais".
 *      - Valores entre ao menos: 0 e 65535.
 *      - Geralmente tem valores entre: 0 e 4294967295.
 *
 *  - signed char: numero inteiro com sinal de 8 bits.
 *      - Valores entre: -128 e 127.
 *
 *  - signed short: numero inteiro com sinal de 16 bits.
 *      - Valores entre: -32768 e 32767.
 *
 *  - signed int: numero inteiro com sinal de 16 ou 32 bits.
 *      - O int tem 16 ou 32 bits dependendo do sistema ou plataforma.
 *      - Valores entre: -32768 e  32767 em 16 bits.
 *      - Valores entre: -2147483648 e  2147483647 em 32 bits.
 *
 *  - signed long: numero inteiro com sinal maior que 16 bits.
 *      - o padrão ANSI C89 define que long deve ao menos ser maior que int ou
 *      - ter o mesmo tamanho, então ele pode ter 29 bits, 64 bits, 12000 bits
 *      - não é garantido um tamanho especifico para ele, nesse codigo ele será
 *      - usado no contexto de "um número com 16 bits ou mais".
 *      - Valores entre ao menos: -32768 e 32767.
 *      - Geralmente tem valores entre: -2147483648 e 2147483647.
 *
 * Com relação a inclusão de bibliotecas externas, eu irei definir um comentário
 * com as cláusulas de inclusão das biblitecas usadas no tipo de dado em questão
 * para inclusão seletiva das bibliotecas, caso queria assegurar todos os tipos
 * copie todas as clausulas de inclusão abaixo deste comentário.
 *
 * Os comentários de dupla barra (//) não existem em C89, apenas os comentários
 * de multi-bloco.
 *
 * Em C89 também não é possivel fazer declaração e associação conjunta, exemplo:
 *  int x = 42;
 * Devemos fazer assim:
 *  int x;
 *  x = 42;
 *
 * Inclusive em Loops.
 *
 *******************************************************************************
 * Eu realmente me interessei na ideia da biblioteca de código, e modifiquei o
 * arquivo dos tipos implementados na aula pra servir como um de exemplo de como
 * eu faria a biblioteca, porém é notório que esse codigo é projetado para ser
 * definido em apenas um único arquivo, a biblioteca teria mais arquivos para
 * ser modular, mas enfim, espero que esse arquivo não seja tão confuso kkkk.
 *                                          - Bruno Costa Barreto (vulgo Mirai)
 *******************************************************************************/

/* O arquivo de cabeçalho Standard Input-Output (stdio.h) é usado para funções
   de entrada e saida de dados como printf(), e scanf(), entre outras. */
#include <stdio.h>
/* O arquivo de cabeçalho Standard Library (stdlib.h) é usado para um misto de
   funções, macros e variaveis como EXIT_SUCCESS (macro que equivale a 0), as
   funções exit, malloc(), calloc(), free() e varias outras */
#include <stdlib.h>
/* O arquivo de cabeçalho de string (string.h) é usado para operações referentes
   a esse tipo de dado, como strcmp() (string compare), strcpy() (string copy)
   e varias outras funções. */
#include <string.h>

/*******************************************************************************
 *                           FILA ESTATICA LINEAR
 *******************************************************************************
 * Uma Fila Estatica é um elemento FIFO (First In First Out) ou PEPS (Primeiro a
 * Entrar Primeiro a Sair), que não altera seu tamanho de forma dinamica podendo
 * somente armazernar um numero fixo de items.
 *******************************************************************************/

/* Essa é uma medida de segurança para impedir que MACRO_FILA_ESTATICA_TIPO
   seja re-definido caso tenha sido definido anteriormente */
#ifndef MACRO_FILA_ESTATICA_TIPO
/* Esse código é usado nas diretrizes de pré-processador para dinamicamente
   adaptar o código para o tipo de dado selecionado (Em C não conseguimos fazer
   comparação de textos em macros, somente números) a baixo você encontra a
   tabela com os códigos para cada tipo:
        0 - signed char   4 - unsigned char   8  - char  12 - float
        1 - signed short  5 - unsigned short  9  - short 13 - double
        2 - signed int    6 - unsigned int    10 - int
        3 - signed long   7 - unsigned long   11 - long */
#define MACRO_FILA_ESTATICA_CODIGO 12

/* Esses macros usados através do código para refletir a mudança no tipo da
   estrutura de dados, eles definem as strings de formatação e exibição além de
   guardar o tipo de dado da fila estatica, pois ele será utilizado tanto dentro
   da struct quanto dentro da função sizeof() para alocação do espaço.

   Esse macro é automaticamente definido quando alterarmos o macro acima */
#if MACRO_FILA_ESTATICA_CODIGO == 0
#define MACRO_FILA_ESTATICA_TIPO signed char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhd\n"
#define SCANF_FORMAT "%hhd"
#define SHOW_STRING0 "| %-16lu | %-39hhd "
#define SHOW_STRING1 "| %-36lu | %37hhd "
#define SHOW_STRING2 "| %-36hhd "
#define SHOW_STRING3 "| %-76hhd "
#define SHOW_STRING4 "%hhd"
#elif MACRO_FILA_ESTATICA_CODIGO == 1
#define MACRO_FILA_ESTATICA_TIPO signed short
#define PRINTF_FORMAT_STR "O valor removido foi: %hd\n"
#define SCANF_FORMAT "%hd"
#define SHOW_STRING0 "| %-16lu | %-39hd "
#define SHOW_STRING1 "| %-36lu | %37hd "
#define SHOW_STRING2 "| %-36hd "
#define SHOW_STRING3 "| %-76hd "
#define SHOW_STRING4 "%hd"
#elif MACRO_FILA_ESTATICA_CODIGO == 2
#define MACRO_FILA_ESTATICA_TIPO signed int
#define PRINTF_FORMAT_STR "O valor removido foi: %d\n"
#define SCANF_FORMAT "%d"
#define SHOW_STRING0 "| %-16lu | %-39d "
#define SHOW_STRING1 "| %-36lu | %37d "
#define SHOW_STRING2 "| %-36d "
#define SHOW_STRING3 "| %-76d "
#define SHOW_STRING4 "%d"
#elif MACRO_FILA_ESTATICA_CODIGO == 3
#define MACRO_FILA_ESTATICA_TIPO signed long
#define PRINTF_FORMAT_STR "O valor removido foi: %ld\n"
#define SCANF_FORMAT "%ld"
#define SHOW_STRING0 "| %-16lu | %-39ld "
#define SHOW_STRING1 "| %-36lu | %37ld "
#define SHOW_STRING2 "| %-36ld "
#define SHOW_STRING3 "| %-76ld "
#define SHOW_STRING4 "%ld"
#elif MACRO_FILA_ESTATICA_CODIGO == 4
#define MACRO_FILA_ESTATICA_TIPO unsigned char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhu\n"
#define SCANF_FORMAT "%hhu"
#define SHOW_STRING0 "| %-16lu | %-39hhu "
#define SHOW_STRING1 "| %-36lu | %37hhu "
#define SHOW_STRING2 "| %-36hhu "
#define SHOW_STRING3 "| %-76hhu "
#define SHOW_STRING4 "%hhu"
#elif MACRO_FILA_ESTATICA_CODIGO == 5
#define MACRO_FILA_ESTATICA_TIPO unsigned short
#define PRINTF_FORMAT_STR "O valor removido foi: %hu\n"
#define SCANF_FORMAT "%hu"
#define SHOW_STRING0 "| %-16lu | %-39hu "
#define SHOW_STRING1 "| %-36lu | %37hu "
#define SHOW_STRING2 "| %-36hu "
#define SHOW_STRING3 "| %-76hu "
#define SHOW_STRING4 "%hu"
#elif MACRO_FILA_ESTATICA_CODIGO == 6
#define MACRO_FILA_ESTATICA_TIPO unsigned int
#define PRINTF_FORMAT_STR "O valor removido foi: %u\n"
#define SCANF_FORMAT "%u"
#define SHOW_STRING0 "| %-16lu | %-39u "
#define SHOW_STRING1 "| %-36lu | %37u "
#define SHOW_STRING2 "| %-36u "
#define SHOW_STRING3 "| %-76u "
#define SHOW_STRING4 "%u"
#elif MACRO_FILA_ESTATICA_CODIGO == 7
#define MACRO_FILA_ESTATICA_TIPO unsigned long
#define PRINTF_FORMAT_STR "O valor removido foi: %lu\n"
#define SCANF_FORMAT "%lu"
#define SHOW_STRING0 "| %-16lu | %-39lu "
#define SHOW_STRING1 "| %-36lu | %37lu "
#define SHOW_STRING2 "| %-36lu "
#define SHOW_STRING3 "| %-76lu "
#define SHOW_STRING4 "%lu"
#elif MACRO_FILA_ESTATICA_CODIGO == 8
#define MACRO_FILA_ESTATICA_TIPO char
#define PRINTF_FORMAT_STR "O valor removido foi: %hhd\n"
#define SCANF_FORMAT "%hhd"
#define SHOW_STRING0 "| %-16lu | %-39hhd "
#define SHOW_STRING1 "| %-36lu | %37hhd "
#define SHOW_STRING2 "| %-36hhd "
#define SHOW_STRING3 "| %-76hhd "
#define SHOW_STRING4 "%hhd"
#elif MACRO_FILA_ESTATICA_CODIGO == 9
#define MACRO_FILA_ESTATICA_TIPO short
#define PRINTF_FORMAT_STR "O valor removido foi: %hd\n"
#define SCANF_FORMAT "%hd"
#define SHOW_STRING0 "| %-16lu | %-39hd "
#define SHOW_STRING1 "| %-36lu | %37hd "
#define SHOW_STRING2 "| %-36hd "
#define SHOW_STRING3 "| %-76hd "
#define SHOW_STRING4 "%hd"
#elif MACRO_FILA_ESTATICA_CODIGO == 10
#define MACRO_FILA_ESTATICA_TIPO int
#define PRINTF_FORMAT_STR "O valor removido foi: %d\n"
#define SCANF_FORMAT "%d"
#define SHOW_STRING0 "| %-16lu | %-39d "
#define SHOW_STRING1 "| %-36lu | %37d "
#define SHOW_STRING2 "| %-36d "
#define SHOW_STRING3 "| %-76d "
#define SHOW_STRING4 "%d"
#elif MACRO_FILA_ESTATICA_CODIGO == 11
#define MACRO_FILA_ESTATICA_TIPO long
#define PRINTF_FORMAT_STR "O valor removido foi: %ld\n"
#define SCANF_FORMAT "%ld"
#define SHOW_STRING0 "| %-16lu | %-39ld "
#define SHOW_STRING1 "| %-36lu | %37ld "
#define SHOW_STRING2 "| %-36ld "
#define SHOW_STRING3 "| %-76ld "
#define SHOW_STRING4 "%ld"
#elif MACRO_FILA_ESTATICA_CODIGO == 12
#define MACRO_FILA_ESTATICA_TIPO float
#define PRINTF_FORMAT_STR "O valor removido foi: %f\n"
#define SCANF_FORMAT "%f"
#define SHOW_STRING0 "| %-16lu | %-39f "
#define SHOW_STRING1 "| %-36lu | %37f "
#define SHOW_STRING2 "| %-36f "
#define SHOW_STRING3 "| %-76f "
#define SHOW_STRING4 "%f"
#elif MACRO_FILA_ESTATICA_CODIGO == 13
#define MACRO_FILA_ESTATICA_TIPO double
#define PRINTF_FORMAT_STR "O valor removido foi: %f\n"
#define SCANF_FORMAT "%f"
#define SHOW_STRING0 "| %-16lu | %-39f "
#define SHOW_STRING1 "| %-36lu | %37f "
#define SHOW_STRING2 "| %-36f "
#define SHOW_STRING3 "| %-76f "
#define SHOW_STRING4 "%f"
#else
#error "Tipo de MACRO_FILA_ESTATICA_TIPO não reconhecido"
#error "Tipo Não Reconhecido para SCANF_FORMAT"
#error "Tipo Não Reconhecido para MACRO_FILA_ESTATICA_TIPO"
#error "Tipo Não Reconhecido para PRINTF_FORMAT_STR"
#endif
#endif

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

/* essa função exibe a fila estatica, o parametro de modificadores altera o modo
   de exibição da lista, ele utiliza mascara de bits para selecionar os valores.
   modificadores:
    - (0b10000000 - 1 bit) exibir conectores: exibe uma seta "->" entre os
        valores caso o bit seja zero, irá exibir uma virgula no lugar.
    - (0b01000000 - 2 bit) exibir endereços inicio, atual e fim: exibe os
        endereços do inicio e do fim sempre e exibe o endereço do valor
        adicionado ou removido da lista quando executado.
    - (0b00100000 - 3 bit) exibir como tabela: exibe a fila em forma de tabela
        incompativel com o modificador do bit 1.
    - (0b00010000 - 4 bit) exibir todos os endereços: modificador da forma de
        tabelas, exibe os endereços na memória de todos os valores.
    - (0b00001000 - 5 bit) exibir todas as posições: modificador da forma de
        tabelas, exibe as posições (indices) dos elementos.
    - (0b00010000 - 6 bit) exibir lista completa: exibe todos os valores
        inclusive aqueles que foram removidos, e não somente os valores
        significativos da fila.
    - (bits 7 a 8): reservado.
   como você define esses modificadores?
    usando o operador OR (|), exemplo:
        (1 << 0) | (1 << 3) | (1 << 5)
        define os modficadores 1, 4 e 6, o indice dos modificadores começa em 0.
    definindo somente 0 usará o padrão (fila com separador de virgula).
   exemplos de uso:
        exibir_fila_estatica(fila_ptr, (1 << 0) | (1 << 3) | (1 << 5));
        exibir_fila_estatica(fila_ptr, 0);

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
void exibir_fila_estatica(fila_estatica_t *fila_ptr,
                          unsigned char modificadores);

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

/* essa função testa de forma automatica a fila estatica com valores aleatorios
 */
void testar_fila_estatica(void);

/* essa função testa a fila estatica de forma interativa com um menu. */
void testar_fila_estatica_interativo(void);

/* Implementação das funções */
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

void exibir_fila_estatica(fila_estatica_t *fila_ptr,
                          unsigned char modificadores) {
    unsigned long i;
    int exibir_conectores;
    int exibir_enderecos;
    int exibir_como_tabela;
    int exibir_todos_enderecos;
    int exibir_todas_posicoes;
    int exibir_todos_os_valores;

    if (!fila_ptr || !fila_ptr->dados_ptr) {
        printf("Fila não inicializada.\n");
        return;
    }

    /* Variáveis de controle para o layout de exibição.
       usando o operador bitwise-booleano AND podemos fazer bit-masking.
       Exemplo:
            00000001 &
            01011001
            --------
            00000001 != 0 == verdadeiro.
       Bit-masking é usar uma sequencia de bits e "uma mascara" para saber se
       o bit desejado está "setado" ou não.
       isso é essencialmente uma array de booleanos. */
    exibir_conectores = (modificadores & (1 << 0)) != 0;
    exibir_enderecos = (modificadores & (1 << 1)) != 0;
    exibir_como_tabela = (modificadores & (1 << 2)) != 0;
    exibir_todos_enderecos = (modificadores & (1 << 3)) != 0;
    exibir_todas_posicoes = (modificadores & (1 << 4)) != 0;
    exibir_todos_os_valores = (modificadores & (1 << 5)) != 0;

    if (exibir_como_tabela) {
        /* Exibe o cabeçalho da tabela. */
        if (exibir_todas_posicoes) {
            printf("/----------------------------------------------------------"
                   "--------------------\\\n");
            if (exibir_todos_enderecos || exibir_enderecos) {
                printf("|%-18s|", " INDICE");
                printf("%-41s|", " VALOR");
                printf("%18s|", "ENDEREÇO ");
                printf("\n|------------------+---------------------------------"
                       "--------+-----------------|\n");
            } else {
                printf("|%-38s|", " INDICE");
                printf("%39s|", "VALOR ");
                printf("\n|--------------------------------------+-------------"
                       "--------------------------|\n");
            }

            if (exibir_todos_os_valores) {
                /* Exibe cada elemento como uma linha da tabela. */
                for (i = fila_ptr->inicio; i < fila_ptr->capacidade; i++) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf(SHOW_STRING0, i, fila_ptr->dados_ptr[i]);
                        printf("| %15p |", (void *)&fila_ptr->dados_ptr[i]);
                    } else {
                        printf(SHOW_STRING1, i, fila_ptr->dados_ptr[i]);
                        printf("|");
                    }
                    printf("\n");
                }
            } else {
                if (exibir_todos_os_valores) {
                    /* Exibe cada elemento como uma linha da tabela. */
                    for (i = fila_ptr->inicio; i < fila_ptr->capacidade; i++) {
                        if (exibir_todos_enderecos || exibir_enderecos) {
                            printf(SHOW_STRING0, i, fila_ptr->dados_ptr[i]);
                            printf("| %15p |", (void *)&fila_ptr->dados_ptr[i]);
                        } else {
                            printf(SHOW_STRING1, i, fila_ptr->dados_ptr[i]);
                            printf("|");
                        }
                        printf("\n");
                    }
                } else {
                    /* Exibe cada elemento como uma linha da tabela. */
                    for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                        if (exibir_todos_enderecos || exibir_enderecos) {
                            printf(SHOW_STRING0, i, fila_ptr->dados_ptr[i]);
                            printf("| %15p |", (void *)&fila_ptr->dados_ptr[i]);
                        } else {
                            printf(SHOW_STRING1, i, fila_ptr->dados_ptr[i]);
                            printf("|");
                        }
                        printf("\n");
                    }
                }
            }

            printf("\\---------------------------------------------------------"
                   "---------------------/\n");
        } else {
            printf("/----------------------------------------------------------"
                   "--------------------\\\n");
            if (exibir_todos_enderecos || exibir_enderecos) {
                printf("|%-38s|", " VALOR");
                printf("%40s|", "ENDEREÇO ");
                printf("\n|--------------------------------------+-------------"
                       "--------------------------|\n");
            } else {
                printf("|%-78s|", " VALOR");
                printf("\n|----------------------------------------------------"
                       "--------------------------|\n");
            }

            /* Exibe cada elemento como uma linha da tabela. */
            for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                if (exibir_todos_enderecos || exibir_enderecos) {
                    printf(SHOW_STRING2, fila_ptr->dados_ptr[i]);
                    printf("| %37p |", (void *)&fila_ptr->dados_ptr[i]);
                } else {
                    printf(SHOW_STRING3, fila_ptr->dados_ptr[i]);
                    printf("|");
                }
                printf("\n");
            }
            printf("\\---------------------------------------------------------"
                   "---------------------/\n");
        }
    } else {
        if (exibir_todos_os_valores) {
            for (i = 0; i < fila_ptr->capacidade; i++) {
                printf(SHOW_STRING4, fila_ptr->dados_ptr[i]);

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    printf(" (%p)\n", (void *)&fila_ptr->dados_ptr[i]);
                } else {
                    /* Exibe o conector entre elementos. */
                    if (i != (fila_ptr->capacidade - 1)) {
                        printf(exibir_conectores ? " -> " : ", ");
                    }
                }
            }
            printf("\n");
        } else {
            /* Exibe em formato de lista (com separador configurável). */
            for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                printf(SHOW_STRING4, fila_ptr->dados_ptr[i]);

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    printf(" (%p)\n", (void *)&fila_ptr->dados_ptr[i]);
                } else {
                    /* Exibe o conector entre elementos. */
                    if (i != fila_ptr->final - 1) {
                        printf(exibir_conectores ? " -> " : ", ");
                    }
                }
            }
            printf("\n");
        }
    }
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

void testar_fila_estatica(void) {
    fila_estatica_t *fila;
    fila = criar_fila_estatica_ret(8);
    inserir_valor_fila_estatica_ret(fila, 99);
    inserir_valor_fila_estatica_ret(fila, 333333);
    inserir_valor_posicao_fila_estatica(fila, 4, 777);
    inserir_valor_fila_estatica_ret(fila, 44);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);

    remover_valor_fila_estatica_ret(fila);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);
    remover_valor_fila_estatica_ret(fila);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);
    remover_valor_fila_estatica_ret(fila);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);
    remover_valor_fila_estatica_ret(fila);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);
    realocar_mover_fila_estatica(fila);
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, 0);

    reiniciar_fila_estatica(fila);
    exibir_fila_estatica(fila, (1 << 5));

    inserir_valor_fila_estatica_ret(fila, 99);
    inserir_valor_fila_estatica_ret(fila, 333333);
    inserir_valor_fila_estatica_ret(fila, 1);
    inserir_valor_fila_estatica_ret(fila, 2);
    inserir_valor_fila_estatica_ret(fila, 3);
    exibir_fila_estatica(fila, (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) |
                                   (1 << 4) | (1 << 5));
    exibir_fila_estatica(fila, (1 << 0) | (1 << 2) | (1 << 4) | (1 << 5));
    exibir_fila_estatica(fila,
                         (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5));
    exibir_fila_estatica(fila, (1 << 0) | (1 << 2) | (1 << 5));
    exibir_fila_estatica(fila, (1 << 0) | (1 << 5));
    exibir_fila_estatica(fila, (1 << 5));
    exibir_fila_estatica(fila, (1 << 1) | (1 << 5));
    exibir_fila_estatica(fila, 0);
    exibir_fila_estatica(fila, (1 << 1));
    remover_valor_posicao_fila_estatica(fila, 3);
    exibir_fila_estatica(fila, (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) |
                                   (1 << 4) | (1 << 5));

    destruir_fila_estatica_ptr(fila);
}

void testar_fila_estatica_interativo(void) {
    static unsigned char escolha;
    static unsigned char escolha_exibicao;
    static unsigned char opcoes_exibicao;
    static unsigned char exibir_ao_mudar;
    static unsigned long capacidade;
    static unsigned char posicao_valida_adicionar;
    static MACRO_FILA_ESTATICA_TIPO valor_pos;
    static unsigned long pos;
    static fila_estatica_t *fila;
    static MACRO_FILA_ESTATICA_TIPO valor_removido_pos;
    static MACRO_FILA_ESTATICA_TIPO valor;
    static MACRO_FILA_ESTATICA_TIPO valor_removido;
    static unsigned char posicao_valida_remover;
    static unsigned long remover_posicao;

    fila = NULL;
    opcoes_exibicao = 0;
    exibir_ao_mudar = 0;
    posicao_valida_adicionar = 1;
    posicao_valida_remover = 1;

/* Usamos diretrizes do pré-processador para definir o comando correto
   para limpar a saida do terminal. */
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    do {
        printf("Menu interativo de teste e exibição da fila linear estatica\n");
        printf("Selecione sua opção digitando o numero entre colchetes e "
               "pressionando ENTER.\n");
        printf("[1] - Criar Lista\n");
        printf("[2] - Adicionar elemento\n");
        printf("[3] - Adicionar elemento na posição\n");
        printf("[4] - Remover elemento\n");
        printf("[5] - Remover elemento na posição\n");
        printf("[6] - Reiniciar Fila\n");
        printf("[7] - Re-alocar elementos\n");
        printf("[8] - Configurar Exibição da lista\n");
        printf("[9] - Exibir Lista\n");
        printf("[10] - Exibir Ajuda\n");
        printf("[0] - Sair\n");
        printf("\n\n");

        printf("Insira sua escolha: ");
        scanf("%hhu", &escolha);
        printf("\n");

        switch (escolha) {
        case 1:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[1] CRIAR LISTA\n\n");
            printf("Insira a capacidade da lista: ");
            scanf("%lu", &capacidade);
            if (fila == NULL) {
                fila = criar_fila_estatica_ret(capacidade);
            } else {
                destruir_fila_estatica_ptr(fila);
                fila = criar_fila_estatica_ret(capacidade);
            }

/* Usamos diretrizes do pré-processador para definir o comando correto
   para limpar a saida do terminal. */
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf(
                "FILA ESTATICA LINEAR CRIADA COM CAPACIDADE PARA %lu ITEMS.\n",
                capacidade);
            if (exibir_ao_mudar != 0) {
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 2:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[2] ADICIONAR ELEMENTO\n\n");

            printf("Insira o valor desejado: ");
            scanf(SCANF_FORMAT, &valor);

            inserir_valor_fila_estatica_ret(fila, valor);

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            if (exibir_ao_mudar != 0) {
                if ((opcoes_exibicao & (1 << 1)) != 0) {
                    printf("Endereço de Inicio: %p\n",
                           (void *)(&fila->dados_ptr[fila->inicio]));
                    printf("Endereço do Elemento: %p\n",
                           (void *)(&fila->dados_ptr[fila->final]));
                    printf("Endereço do Final: %p\n\n",
                           (void *)(&fila->dados_ptr[fila->final + 1]));
                }
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 3:
            do {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                printf("[3] ADICIONAR ELEMENTO NA POSIÇÃO\n\n");

                printf("Insira a posição desejada: ");
                scanf("%lu", &pos);

                if (pos <= fila->final && pos >= fila->inicio) {
                    posicao_valida_adicionar = !posicao_valida_adicionar;
                }
            } while (posicao_valida_adicionar != 0);

            printf("Insira o valor desejado: ");
            scanf(SCANF_FORMAT, &valor_pos);

            inserir_valor_posicao_fila_estatica(fila, pos, valor_pos);

            if (exibir_ao_mudar != 0) {
                if ((opcoes_exibicao & (1 << 1)) != 0) {
                    printf("Endereço de Inicio: %p\n",
                           (void *)(&fila->dados_ptr[fila->inicio]));
                    printf("Endereço do Elemento: %p\n",
                           (void *)(&fila->dados_ptr[pos]));
                    printf("Endereço do Final: %p\n\n",
                           (void *)(&fila->dados_ptr[fila->final + 1]));
                }
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 4:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[4] REMOVER ELEMENTO\n\n");

            valor_removido = remover_valor_fila_estatica_ret(fila);

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            printf(PRINTF_FORMAT_STR, valor_removido);

            if (exibir_ao_mudar != 0) {
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 5:
            do {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                printf("[5] REMOVER ELEMENTO NA POSIÇÃO\n\n");

                printf("Insira a posição desejada: ");
                scanf("%lu", &remover_posicao);
                if (remover_posicao <= fila->final &&
                    remover_posicao >= fila->inicio) {
                    posicao_valida_remover = !posicao_valida_remover;
                }
            } while (posicao_valida_remover != 0);

            valor_removido_pos =
                remover_valor_posicao_fila_estatica(fila, remover_posicao);

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            printf(PRINTF_FORMAT_STR, valor_removido_pos);

            if (exibir_ao_mudar != 0) {
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 6:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[6] REINICIAR FILA\n\n");

            reiniciar_fila_estatica(fila);

            if (exibir_ao_mudar != 0) {
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 7:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[6] REALOCAR ELEMENTOS\n\n");

            realocar_mover_fila_estatica(fila);

            if (exibir_ao_mudar != 0) {
                exibir_fila_estatica(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 8:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            do {
                printf("[8] CONFIGURAR OPÇÕES DE EXIBIÇÃO\n\n");
                printf("Selecione o valor que deseja alterar digitando o "
                       "numero entreo parenteses e digitando ENTER\n");
                printf("esse programa alterna os valores, então valores que "
                       "forem definidos como\n");
                printf(
                    "verdadeiro serão transformados em falso e vice-versa.\n");
                printf("Lista de opções:\n");
                printf("\t(1) - Exibir conectores [%s]\n",
                       ((opcoes_exibicao & (1 << 0)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe -> ao invés de virgulas na exibição não "
                       "tabular da lista.\n");
                printf(
                    "\t(2) - Exibir endereço do inicio, atual e final [%s]\n",
                    ((opcoes_exibicao & (1 << 1)) != 0) ? "SELECIONADO"
                                                        : "DISPONIVEL");
                printf("\t\tExibe os endereços dos elementos a cada ação\n");
                printf("\t(3) - Exibir como tabela [%s]\n",
                       ((opcoes_exibicao & (1 << 2)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe a lista como uma tabela\n");
                printf("\t(4) - Exibir todos os endereços [%s]\n",
                       ((opcoes_exibicao & (1 << 3)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe os endereços nas visões tabular e linear da "
                       "lista\n");
                printf("\t(5) - Exibir posições [%s]\n",
                       ((opcoes_exibicao & (1 << 4)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe os indices na visão tabular da lista\n");
                printf("\t(6) - Exibir lista completa [%s]\n",
                       ((opcoes_exibicao & (1 << 5)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe todos os valores, mesmo que os espaços "
                       "estejam em branco.\n");
                printf("\t(9) - Exibir lista em todas as ações [%s]\n",
                       (exibir_ao_mudar != 0) ? "SELECIONADO" : "DISPONIVEL");
                printf("\t\tExibe a lista em todas as ações, nota-se que se "
                       "não definir a opção (6), caso a lista esteja em "
                       "branco, nada será exibido.\n");
                printf("\t(0) - Voltar ao menu principal\n");
                printf("\t\tFecha esse menu e retorna ao menu principal.\n");
                printf("\n\n");
                printf("Insira sua escolha: ");
                scanf("%hhu", &escolha_exibicao);
                printf("\n");

                switch (escolha_exibicao) {
                case 1:
                    opcoes_exibicao ^= (1 << 0);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 2:
                    opcoes_exibicao ^= (1 << 1);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 3:
                    opcoes_exibicao ^= (1 << 2);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 4:
                    opcoes_exibicao ^= (1 << 3);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 5:
                    opcoes_exibicao ^= (1 << 4);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 6:
                    opcoes_exibicao ^= (1 << 5);
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 9:
                    exibir_ao_mudar = !exibir_ao_mudar;
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                case 0:
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    break;
                default:
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    printf("OPÇÃO INVALIDA\n\n");
                    break;
                }
            } while (escolha_exibicao != 0);
            break;
        case 9:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[9] EXIBIR LISTA\n\n");
            exibir_fila_estatica(fila, opcoes_exibicao);
            break;
        case 10:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Você utiliza os números entre colchetes para se movimentar "
                   "na aplicação\n");
            printf("A primeira opção cria uma fila ou sobreescreve uma fila "
                   "previamente criada\n");
            printf("A segunda opção você insere um valor referente ao tipo da "
                   "fila na ultima posição\n");
            printf("A terceira opção você insere um valor em uma dada posição "
                   "dentro dos limites da fila (pode ser computacionalmente "
                   "caro)\n");
            printf("A quarta opção você remove o valor no inicio da fila\n");
            printf(
                "A quinta opção remove um valor em uma dada posição e reajusta "
                "todos os valores (pode ser computacionalmente caro)\n");
            printf("A sexta opção reinicia a fila e limpa as celulas de "
                   "memória, reciclando a fila sem criar uma nova (alocação de "
                   "memória é custoso)\n");
            printf(
                "A sétima re-aloca os elementos para alinhar novamente a fila "
                "e as celulas de memoria (pode ser computacionalmente caro)\n");
            printf("A oitava opção configura todas as opções de exibição da "
                   "lista, inclusive a exibição da mesma em todas as ações "
                   "(recomendo acessar antes de criar a lista)\n");
            printf("A nona opção exibe este menu\n");
            printf(
                "A última opção sai do programa e limpa toda memória usada.\n");
            break;
        case 0:
            printf("Saindo.\n");
            break;
        default:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Opção Invalida, favor tentar novamente.\n\n");
            break;
        }

        printf("\n");

    } while (escolha != 0);

    if (fila != NULL) {
        destruir_fila_estatica_ptr(fila);
    }
}

/*******************************************************************************
 *                             Programa Principal
 *******************************************************************************
 * Abaixo você encontrará o o programa principal com um sistema de menus para os
 * diferentes tipos de dado implementados nesse arquivo
 *******************************************************************************/

/* em ANSI C89 funções sem parametros devem receber void, pois o compilador não
   deduz que a função não tem parametros, então é necessária a declaração
   explicita de "nada". */
int main(void) {
    unsigned char escolha_global = 0;
    unsigned char escolha_interativo = 0;

    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("/--------------------------------------------------------------"
               "----------------\\\n");
        printf("|          Banco de Código da Universidade do Estado do Rio de "
               "Janeiro         |\n");
        printf("|%78s|\n", "");
        printf("| Este programa tem o objetivo auxiliar na compreensão de "
               "estruturas de dado   |\n");
        printf("| usando a linguagem C (padrão ANSI 1989) de programação, "
               "quaisquer dúvidas    |\n");
        printf("| contate o professor ou mande um email ou mensagem para o "
               "contato ao final    |\n");
        printf("| Este programa é de código livre e aberto dísponivel no "
               "seguinte endereço web |\n");
        printf("| https://github.com/MiraiMindz/banca_de_codigo_uerj           "
               "                |\n");
        printf("|%78s|\n", "");
        printf("|--------------------------------------------------------------"
               "----------------|\n");
        printf("|%78s|\n", "");
        printf("| Escrito por Bruno C. Barreto (vulgo Mirai)                   "
               "                |\n");
        printf("|   - Email de Contato: barreto2contato@gmail.com              "
               "                |\n");
        printf("|   - Telefone Celular de Contato: +55 (21) 97157-0580         "
               "                |\n");
        printf("|%78s|\n", "");
        printf("\\-------------------------------------------------------------"
               "-----------------/\n\n");
        printf("[1] - Fila Linear Estatica\n");
        printf("[0] - Sair\n\n");

        printf("Insira sua Resposta: ");
        scanf("%hhu", &escolha_global);

        switch (escolha_global) {
        case 1:
            do {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                printf("Escolha a forma que deseja executar a Fila Linear "
                       "Estatica:\n");
                printf("[1] - Automatica \n");
                printf("[2] - Interativa \n");
                printf("[0] - Sair \n\n");

                printf("Insira sua resposta: ");
                scanf("%hhu", &escolha_interativo);

                switch (escolha_interativo) {
                case 1:
                    testar_fila_estatica();
                    break;
                case 2:
                    testar_fila_estatica_interativo();
                case 0:
                    printf("Voltando para o menu principal\n");
                    break;
                default:
                    printf("Opção Invalida\n");
                    break;
                }
            } while (escolha_interativo != 0);
            break;
        case 0:
            break;
        default:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Opção Invalida, favor tentar novamente.\n\n");
            break;
        }
    } while (escolha_global != 0);
    return 0;
}
