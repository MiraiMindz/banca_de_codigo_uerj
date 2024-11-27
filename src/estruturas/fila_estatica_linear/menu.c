#include "estruturas/fila_estatica_linear/menu.h"
#include <stdio.h>
#include <stdlib.h>

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

void testar_fila_estatica_automatico(void) {
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



void fila_estatica_menu_interativo(void) {
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
