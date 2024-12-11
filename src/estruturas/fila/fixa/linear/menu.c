/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#include <estruturas/fila/fixa/linear/menu.h>
#include <internal/cli.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

void exibir_fila_fixa_linear(fila_fixa_linear_t *fila_ptr,
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
                printf("|");
                cli_definir_cores(VERDE_CLARO, PADRAO);
                printf("%-18s", " INDICE");
                cli_redefinir_cores();
                printf("|");
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf("%-41s", " VALOR");
                cli_redefinir_cores();
                printf("|");
                cli_definir_cores(AZUL_CLARO, PADRAO);
                printf("%18s", "ENDEREÇO ");
                cli_redefinir_cores();
                printf("|");
                printf("\n|------------------+---------------------------------"
                       "--------+-----------------|\n");
            } else {
                printf("|");
                cli_definir_cores(VERDE_CLARO, PADRAO);
                printf("%-38s", " INDICE");
                cli_redefinir_cores();
                printf("|");
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf("%39s", "VALOR ");
                cli_redefinir_cores();
                printf("|");
                printf("\n|--------------------------------------+-------------"
                       "--------------------------|\n");
            }

            if (exibir_todos_os_valores) {
                /* Exibe cada elemento como uma linha da tabela. */
                for (i = 0; i < fila_ptr->capacidade; i++) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf("| ");
                        cli_definir_cores(VERDE_CLARO, PADRAO);
                        printf(SHOW_STRING0A, i);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING0B, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf("%15p", (void *)&fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    } else {
                        printf("| ");
                        cli_definir_cores(VERDE_CLARO, PADRAO);
                        printf(SHOW_STRING1A, i);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING1B, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    }
                    printf("\n");
                }
            } else {
                /* Exibe cada elemento como uma linha da tabela. */
                for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf("| ");
                        cli_definir_cores(VERDE_CLARO, PADRAO);
                        printf(SHOW_STRING0A, i);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING0B, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf("%15p", (void *)&fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    } else {
                        printf("| ");
                        cli_definir_cores(VERDE_CLARO, PADRAO);
                        printf(SHOW_STRING1A, i);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING1B, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
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
                printf("|");
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf("%-38s", " VALOR");
                cli_redefinir_cores();
                printf("|");
                cli_definir_cores(AZUL_CLARO, PADRAO);
                printf("%40s", "ENDEREÇO ");
                cli_redefinir_cores();
                printf("|");
                printf("\n|--------------------------------------+-------------"
                       "--------------------------|\n");
            } else {
                printf("|");
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf("%-78s", " VALOR");
                cli_redefinir_cores();
                printf("|");
                printf("\n|----------------------------------------------------"
                       "--------------------------|\n");
            }

            /* Exibe cada elemento como uma linha da tabela. */
            if (exibir_todos_os_valores) {
                for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf("| ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING2, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf("%37p", (void *)&fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    } else {
                        printf("| ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf(SHOW_STRING3, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    }
                    printf("\n");
                }
            } else {
                for (i = 0; i < fila_ptr->capacidade; i++) {
                    if (exibir_todos_enderecos || exibir_enderecos) {
                        printf("| ");
                        cli_definir_cores(BRANCO_CLARO, PADRAO);
                        printf(SHOW_STRING2, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" | ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf("%37p", (void *)&fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    } else {
                        printf("| ");
                        cli_definir_cores(AZUL_CLARO, PADRAO);
                        printf(SHOW_STRING3, fila_ptr->dados_ptr[i]);
                        cli_redefinir_cores();
                        printf(" |");
                    }
                    printf("\n");
                }
            }
            printf("\\---------------------------------------------------------"
                   "---------------------/\n");
        }
    } else {
        if (exibir_todos_os_valores) {
            for (i = 0; i < fila_ptr->capacidade; i++) {
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf(SHOW_STRING4, fila_ptr->dados_ptr[i]);
                cli_redefinir_cores();

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    cli_definir_cores(AZUL_CLARO, PADRAO);
                    printf(" (%p)", (void *)&fila_ptr->dados_ptr[i]);
                    cli_redefinir_cores();
                    printf("\n");
                } else {
                    /* Exibe o conector entre elementos. */
                    if (i != (fila_ptr->capacidade - 1)) {
                        printf(exibir_conectores ? " -> " : ", ");
                    }
                }
            }
            printf("\n");
        } else {
            /* Exibe em formato de fila (com separador configurável). */
            for (i = fila_ptr->inicio; i < fila_ptr->final; i++) {
                cli_definir_cores(BRANCO_CLARO, PADRAO);
                printf(SHOW_STRING4, fila_ptr->dados_ptr[i]);
                cli_redefinir_cores();

                /* Exibe o endereço se configurado. */
                if (exibir_enderecos) {
                    cli_definir_cores(AZUL_CLARO, PADRAO);
                    printf(" (%p)", (void *)&fila_ptr->dados_ptr[i]);
                    cli_redefinir_cores();
                    printf("\n");
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

void fila_fixa_menu_interativo(void) {
    unsigned char escolha;
    unsigned char escolha_exibicao;
    unsigned char opcoes_exibicao;
    unsigned char exibir_ao_mudar;
    unsigned long capacidade;
    fila_fixa_linear_t *fila;
    MACRO_FILA_FIXA_LINEAR_TIPO valor;
    MACRO_FILA_FIXA_LINEAR_TIPO valor_removido;

    fila = NULL;
    opcoes_exibicao = 0;
    exibir_ao_mudar = 0;
    capacidade = 0;

    /* Usamos diretrizes do pré-processador para definir o comando correto
       para limpar a saida do terminal. */

    limpar_tela();
    do {
        printf("Menu interativo de teste e exibição da fila linear de alocação "
               "dinamica e tamanho fixo\n");
        printf("Selecione sua opção digitando o numero entre colchetes e "
               "pressionando ENTER.\n\n");
        printf("[1] - Criar fila\n");
        printf("[2] - Adicionar elemento\n");
        printf("[3] - Remover elemento\n");
        printf("[4] - Reiniciar fila\n");
        printf("[5] - Re-alocar elementos\n");
        printf("[6] - Configurar exibição da fila\n");
        printf("[7] - Exibir fila\n");
        printf("[8] - Exibir Ajuda\n");
        printf("[0] - Sair\n");
        printf("\n\n");

        printf("Insira sua escolha: ");
        scanf("%hhu", &escolha);
        printf("\n");

        switch (escolha) {
        case 1:
            limpar_tela();
            do {
                printf("[1] CRIAR FILA\n\n");
                printf("Insira a capacidade da fila: ");
                scanf("%lu", &capacidade);

                if (capacidade <= 0) {
                    limpar_tela();
                    printf("A capacidade deve ser um número positivo maior que "
                           "zero\n\n");
                    continue;
                } else {
                    if (fila == NULL) {
                        fila = criar_fila_fixa_linear_ret(capacidade);
                    } else {
                        destruir_fila_fixa_linear_ptr(fila);
                        fila = criar_fila_fixa_linear_ret(capacidade);
                    }

                    /* Usamos diretrizes do pré-processador para definir o
                    comando correto para limpar a saida do terminal. */
                    limpar_tela();
                    printf("FILA FIXA LINEAR CRIADA COM CAPACIDADE PARA %lu "
                           "ITEMS.\n",
                           capacidade);
                    if (exibir_ao_mudar != 0) {
                        exibir_fila_fixa_linear(fila, opcoes_exibicao);
                    }
                    printf("\n");
                    break;
                }
            } while (capacidade <= 0);
            capacidade = 0;
            break;
        case 2:
            limpar_tela();
            printf("[2] ADICIONAR ELEMENTO\n\n");

            printf("Insira o valor desejado: ");
            scanf(SCANF_FORMAT, &valor);

            inserir_valor_fila_fixa_linear_ret(fila, valor);

            limpar_tela();

            if (exibir_ao_mudar != 0) {
                if ((opcoes_exibicao & (1 << 1)) != 0) {
                    printf("Endereço de Inicio: %p\n",
                           (void *)(&fila->dados_ptr[fila->inicio]));
                    printf("Endereço do Elemento: %p\n",
                           (void *)(&fila->dados_ptr[fila->final]));
                    printf("Endereço do Final: %p\n\n",
                           (void *)(&fila->dados_ptr[fila->final + 1]));
                }
                exibir_fila_fixa_linear(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 3:
            limpar_tela();
            printf("[3] REMOVER ELEMENTO\n\n");

            valor_removido = remover_valor_fila_fixa_linear_ret(fila);

            limpar_tela();

            printf(PRINTF_FORMAT_STR, valor_removido);

            if (exibir_ao_mudar != 0) {
                if ((opcoes_exibicao & (1 << 1)) != 0) {
                    printf("Endereço de Inicio: %p\n",
                           (void *)(&fila->dados_ptr[fila->inicio]));
                    printf("Endereço do Elemento: %p\n",
                           (void *)(&valor_removido));
                    printf("Endereço do Final: %p\n\n",
                           (void *)(&fila->dados_ptr[fila->final + 1]));
                }
                exibir_fila_fixa_linear(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 4:
            limpar_tela();
            printf("[4] REINICIAR FILA\n\n");

            reiniciar_fila_fixa_linear(fila);

            if (exibir_ao_mudar != 0) {
                exibir_fila_fixa_linear(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 5:
            limpar_tela();
            printf("[5] REALOCAR ELEMENTOS\n\n");

            realocar_mover_fila_fixa_linear(fila);

            if (exibir_ao_mudar != 0) {
                exibir_fila_fixa_linear(fila, opcoes_exibicao);
            }
            printf("\n");
            break;
        case 6:
            limpar_tela();

            do {
                printf("[6] CONFIGURAR OPÇÕES DE EXIBIÇÃO\n\n");
                printf("Selecione o valor que deseja alterar digitando o "
                       "numero entre parenteses e digitando ENTER\n");
                printf("esse programa alterna os valores, então valores que "
                       "forem definidos como\n");
                printf(
                    "verdadeiro serão transformados em falso e vice-versa.\n");
                printf("Lista de opções:\n");
                exibir_opcao_configurar_display(1, "Exibir conectores", "Exibe -> ao invés de virgulas na exibição não tabular da fila.", (opcoes_exibicao & (1 << 0)));
                exibir_opcao_configurar_display(2, "Exibir endereço do inicio, atual e final", "Exibe os endereços dos elementos a cada ação", (opcoes_exibicao & (1 << 1)));
                exibir_opcao_configurar_display(3, "Exibir como tabela", "Exibe a fila como tabela", (opcoes_exibicao & (1 << 2)));
                exibir_opcao_configurar_display(4, "Exibir todos os endereços", "Exibe os endereços nas visões tabular e linear da fila", (opcoes_exibicao & (1 << 3)));
                exibir_opcao_configurar_display(5, "Exibir posições", "Exibe os indices na visão tabular da fila", (opcoes_exibicao & (1 << 4)));
                exibir_opcao_configurar_display(6, "Exibir fila completa", "Exibe todos os valores, mesmo que estejam em branco", (opcoes_exibicao & (1 << 5)));
                exibir_opcao_configurar_display(9, "Exibir a fila em todas as ações", "Exibe a fila em todas as ações, nota-se que se não definir a opção (6), caso a fila esteja em branco, nada será exibido ao criar a lista", exibir_ao_mudar);
                printf("\t(0) - Voltar ao menu principal\n");
                printf("\t\tFecha esse menu e retorna ao menu principal.\n");
                printf("\n\n");
                printf("Insira sua escolha: ");
                scanf("%hhu", &escolha_exibicao);
                printf("\n");

                switch (escolha_exibicao) {
                case 1:
                    opcoes_exibicao ^= (1 << 0);
                    limpar_tela();
                    break;
                case 2:
                    opcoes_exibicao ^= (1 << 1);
                    limpar_tela();
                    break;
                case 3:
                    opcoes_exibicao ^= (1 << 2);
                    limpar_tela();
                    break;
                case 4:
                    opcoes_exibicao ^= (1 << 3);
                    limpar_tela();
                    break;
                case 5:
                    opcoes_exibicao ^= (1 << 4);
                    limpar_tela();
                    break;
                case 6:
                    opcoes_exibicao ^= (1 << 5);
                    limpar_tela();
                    break;
                case 9:
                    exibir_ao_mudar = !exibir_ao_mudar;
                    limpar_tela();
                    break;
                case 0:
                    limpar_tela();
                    break;
                default:
                    limpar_tela();
                    printf("OPÇÃO INVALIDA\n\n");
                    break;
                }
            } while (escolha_exibicao != 0);
            break;
        case 7:
            limpar_tela();
            printf("[7] EXIBIR FILA\n\n");
            exibir_fila_fixa_linear(fila, opcoes_exibicao);
            break;
        case 8:
            limpar_tela();
            printf("Você utiliza os números entre colchetes para se movimentar "
                   "na aplicação\n");
            printf("A primeira opção cria uma fila ou sobreescreve uma fila "
                   "previamente criada\n");
            printf("A segunda opção você insere um valor referente ao tipo da "
                   "fila na ultima posição\n");
            printf("A terceira opção você remove o valor no inicio da fila\n");
            printf("A quarta opção reinicia a fila e limpa as celulas de "
                   "memória, reciclando a fila sem criar uma nova (alocação de "
                   "memória é custoso)\n");
            printf(
                "A quinta re-aloca os elementos para alinhar novamente a fila "
                "e as celulas de memoria (pode ser computacionalmente caro)\n");
            printf("A sexta opção configura todas as opções de exibição da "
                   "fila, inclusive a exibição da mesma em todas as ações "
                   "(recomendo acessar antes de criar a fila)\n");
            printf("A sétima opção exibe a fila usando as opções definidas na "
                   "sexta opção\n");
            printf("A oitava opção exibe este menu\n");
            printf(
                "A última opção sai do programa e limpa toda memória usada.\n");
            break;
        case 0:
            printf("Saindo.\n");
            break;
        default:
            limpar_tela();
            printf("Opção Invalida, favor tentar novamente.\n\n");
            break;
        }

        printf("\n");

    } while (escolha != 0);

    if (fila != NULL) {
        destruir_fila_fixa_linear_ptr(fila);
        fila = NULL;
    }
}
