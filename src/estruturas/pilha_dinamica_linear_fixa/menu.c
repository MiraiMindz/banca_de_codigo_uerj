/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include "estruturas/pilha_dinamica_linear_fixa/menu.h"
#include "estruturas/pilha_dinamica_linear_fixa/estrutura.h"
#include <stdio.h>
#include <stdlib.h>

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
                       unsigned char modificadores) {
    unsigned long i;
    int exibir_conectores;
    int exibir_enderecos;
    int exibir_como_tabela;
    int exibir_todos_enderecos;
    int exibir_todas_posicoes;
    int exibir_todos_os_valores;

    if (!pilha_ptr || !pilha_ptr->dados_ptr) {
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
                for (i = pilha_ptr->capacidade; i > 0; i--) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf(SHOW_STRING0, i - 1, pilha_ptr->dados_ptr[i - 1]);
                        printf("| %15p |", (void *)&pilha_ptr->dados_ptr[i - 1]);
                    } else {
                        printf(SHOW_STRING1, i - 1, pilha_ptr->dados_ptr[i - 1]);
                        printf("|");
                    }
                    printf("\n");
                }
            } else {
                /* Exibe cada elemento como uma linha da tabela. */
                for (i = pilha_ptr->topo; i > 0; i--) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf(SHOW_STRING0, i - 1, pilha_ptr->dados_ptr[i - 1]);
                        printf("| %15p |", (void *)&pilha_ptr->dados_ptr[i - 1]);
                    } else {
                        printf(SHOW_STRING1, i - 1, pilha_ptr->dados_ptr[i - 1]);
                        printf("|");
                    }
                    printf("\n");
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
            if (exibir_todos_os_valores) {
                for (i = pilha_ptr->capacidade; i > 0; i--) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf(SHOW_STRING2, pilha_ptr->dados_ptr[i - 1]);
                        printf("| %37p |", (void *)&pilha_ptr->dados_ptr[i - 1]);
                    } else {
                        printf(SHOW_STRING3, pilha_ptr->dados_ptr[i - 1]);
                        printf("|");
                    }
                    printf("\n");
                }
            } else {
                for (i = pilha_ptr->topo; i > 0; i--) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf(SHOW_STRING2, pilha_ptr->dados_ptr[i - 1]);
                        printf("| %37p |", (void *)&pilha_ptr->dados_ptr[i - 1]);
                    } else {
                        printf(SHOW_STRING3, pilha_ptr->dados_ptr[i - 1]);
                        printf("|");
                    }
                    printf("\n");
                }
            }
            printf("\\---------------------------------------------------------"
                   "---------------------/\n");
        }
    } else {
        if (exibir_todos_os_valores) {
            for (i = pilha_ptr->topo; i > 0; i--) {
                printf(SHOW_STRING4, pilha_ptr->dados_ptr[i - 1]);

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    printf(" (%p)\n", (void *)&pilha_ptr->dados_ptr[i - 1]);
                } else {
                    /* Exibe o conector entre elementos. */
                    if (i != (pilha_ptr->capacidade - 1)) {
                        printf(exibir_conectores ? " -> " : ", ");
                    }
                }
            }
            printf("\n");
        } else {
            /* Exibe em formato de pilha (com separador configurável). */
            for (i = pilha_ptr->capacidade; i > 0; i--) {
                printf(SHOW_STRING4, pilha_ptr->dados_ptr[i - 1]);

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    printf(" (%p)\n", (void *)&pilha_ptr->dados_ptr[i - 1]);
                } else {
                    /* Exibe o conector entre elementos. */
                    if (i != pilha_ptr->topo - 1) {
                        printf(exibir_conectores ? " -> " : ", ");
                    }
                }
            }
            printf("\n");
        }
    }
}

/* essa função testa a pilha fixa de forma interativa com um menu. */
void testar_pilha_fixa_menu_interativo(void) {
    unsigned char escolha_geral;
    unsigned char escolha_exibicao;
    unsigned char opcoes_exibicao;
    unsigned char exibir_ao_mudar;
    unsigned long capacidade;

    pilha_linear_fixa_t *pilha_ptr;
    MACRO_PILHA_FIXA_TIPO valor_inserir;
    MACRO_PILHA_FIXA_TIPO valor_removido;

    pilha_ptr = NULL;
    opcoes_exibicao = 0;
    exibir_ao_mudar = 0;
    capacidade = 0;
    /* Usamos diretrizes do pré-processador para definir o comando correto
para limpar a saida do terminal. */
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    do {
        printf(
            "Menu interativo de teste e exibição da pilha linear de alocação "
            "dinamica e tamanho fixo\n");
        printf("Selecione sua opção digitando o numero entre colchetes e "
               "pressionando ENTER.\n\n");
        printf("[1] - Criar pilha\n");
        printf("[2] - Inserir elemento\n");
        printf("[3] - Retirar elemento\n");
        printf("[4] - Reiniciar pilha\n");
        printf("[5] - Configurar exibição da pilha\n");
        printf("[6] - Exibir pilha\n");
        printf("[7] - Exibir Ajuda\n");
        printf("[0] - Sair\n");
        printf("\n\n");

        printf("Insira sua escolha: ");
        scanf("%hhu", &escolha_geral);
        printf("\n");

        switch (escolha_geral) {
        case 1:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            do {
                printf("[1] CRIAR PILHA\n\n");
                printf("Insira a capacidade da pilha: ");
                scanf("%lu", &capacidade);

                if (capacidade <= 0) {
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    printf("A capacidade deve ser um número positivo maior que "
                           "zero\n\n");
                    continue;
                } else {
                    if (pilha_ptr == NULL) {
                        pilha_ptr = criar_pilha_fixa_ret(capacidade);
                    } else {
                        limpar_pilha_fixa(pilha_ptr);
                        pilha_ptr = criar_pilha_fixa_ret(capacidade);
                    }

#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif

                    printf("PILHA FIXA LINEAR CRIADA COM CAPACIDADE PARA %lu "
                           "ITEMS.\n",
                           capacidade);

                    if (exibir_ao_mudar != 0) {
                        exibir_pilha_fixa(pilha_ptr, opcoes_exibicao);
                    }
                    printf("\n");
                    break;
                }
            } while (capacidade <= 0);
            capacidade = 0;
            break;
        case 2:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            printf("[2] INSERIR ELEMENTO\n\n");
            printf("Insira o valor desejado: ");
            scanf(SCANF_FORMAT, &valor_inserir);

            inserir_pilha_fixa(pilha_ptr, valor_inserir);

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            if (exibir_ao_mudar != 0) {
                if ((opcoes_exibicao & (1 << 1)) != 0) {
                    printf("Endereço de Inicio: %p\n",
                           (void *)(&pilha_ptr->dados_ptr[0]));
                    printf(
                        "Endereço do Elemento: %p\n",
                        (void *)(&pilha_ptr->dados_ptr[pilha_ptr->topo - 1]));
                    printf("Endereço do Final: %p\n\n",
                           (void *)(&pilha_ptr->dados_ptr[pilha_ptr->topo]));
                }
                exibir_pilha_fixa(pilha_ptr, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 3:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            printf("[3] RETIRAR ELEMENTO\n\n");
            if (pilha_ptr->topo == 0) {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                printf("A pilha está vazia, não há nada para remover\n\n");
            } else {
                valor_removido = retirar_pilha_fixa(pilha_ptr);
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif

                printf(PRINTF_FORMAT_STR, valor_removido);

                if (exibir_ao_mudar != 0) {
                    if ((opcoes_exibicao & (1 << 1)) != 0) {
                        printf("Endereço de Inicio: %p\n",
                               (void *)(&pilha_ptr->dados_ptr[0]));
                        printf("Endereço do Elemento: %p\n",
                               (void *)(&valor_removido));
                        printf(
                            "Endereço do Final: %p\n\n",
                            (void *)(&pilha_ptr->dados_ptr[pilha_ptr->topo]));
                    }
                    exibir_pilha_fixa(pilha_ptr, opcoes_exibicao);
                }
                printf("\n");
            }
            break;
        case 4:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[4] REINICIAR PILHA\n\n");

            reiniciar_pilha_fixa(pilha_ptr);

            if (exibir_ao_mudar != 0) {
                exibir_pilha_fixa(pilha_ptr, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 5:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            do {
                printf("[5] CONFIGURAR OPÇÕES DE EXIBIÇÃO\n\n");
                printf("Selecione o valor que deseja alterar digitando o "
                       "numero entre  parenteses e digitando ENTER\n");
                printf("esse programa alterna os valores, então valores que "
                       "forem definidos como\n");
                printf(
                    "verdadeiro serão transformados em falso e vice-versa.\n");
                printf("Lista de opções:\n");
                printf("\t(1) - Exibir conectores [%s]\n",
                       ((opcoes_exibicao & (1 << 0)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe -> ao invés de virgulas na exibição não "
                       "tabular da pilha.\n");
                printf(
                    "\t(2) - Exibir endereço do inicio, atual e final [%s]\n",
                    ((opcoes_exibicao & (1 << 1)) != 0) ? "SELECIONADO"
                                                        : "DISPONIVEL");
                printf("\t\tExibe os endereços dos elementos a cada ação\n");
                printf("\t(3) - Exibir como tabela [%s]\n",
                       ((opcoes_exibicao & (1 << 2)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe a pilha como uma tabela\n");
                printf("\t(4) - Exibir todos os endereços [%s]\n",
                       ((opcoes_exibicao & (1 << 3)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe os endereços nas visões tabular e linear da "
                       "pilha\n");
                printf("\t(5) - Exibir posições [%s]\n",
                       ((opcoes_exibicao & (1 << 4)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe os indices na visão tabular da pilha\n");
                printf("\t(6) - Exibir pilha completa [%s]\n",
                       ((opcoes_exibicao & (1 << 5)) != 0) ? "SELECIONADO"
                                                           : "DISPONIVEL");
                printf("\t\tExibe todos os valores, mesmo que os espaços "
                       "estejam em branco.\n");
                printf("\t(9) - Exibir pilha em todas as ações [%s]\n",
                       (exibir_ao_mudar != 0) ? "SELECIONADO" : "DISPONIVEL");
                printf("\t\tExibe a pilha em todas as ações, nota-se que se "
                       "não definir a opção (6), caso a pilha esteja em "
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
        case 6:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("[6] EXIBIR PILHA\n\n");
            exibir_pilha_fixa(pilha_ptr, opcoes_exibicao);
            break;
        case 7:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Você utiliza os números entre colchetes para se movimentar "
                   "na aplicação\n");
            printf("A primeira opção cria uma pilha ou sobreescreve uma pilha "
                   "previamente criada\n");
            printf("A segunda opção você insere um valor referente ao tipo da "
                   "pilha na ultima posição\n");
            printf("A terceira opção você remove o valor no final da pilha\n");
            printf(
                "A quarta opção reinicia a pilha e limpa as celulas de "
                "memória, reciclando a pilha sem criar uma nova (alocação de "
                "memória é custoso)\n");
            printf("A quinta opção configura todas as opções de exibição da "
                   "pilha, inclusive a exibição da mesma em todas as ações "
                   "(recomendo acessar antes de criar a pilha)\n");
            printf("A sexta opção exibe a pilha usando as opções definidas na "
                   "sexta opção\n");
            printf("A sétima opção exibe este menu\n");
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
            printf("OPÇÃO INVALIDA\n\n");
            break;
        }

    } while (escolha_geral != 0);

    if (pilha_ptr != NULL) {
        limpar_pilha_fixa(pilha_ptr);
        pilha_ptr = NULL;
    }
}