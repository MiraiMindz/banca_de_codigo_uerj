#include "estruturas/fila_estatica_linear/menu.h"
#include "internal/menus.h"

#include <stdio.h>
#include <stdlib.h>

void main_menu(void) {
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
               "contato ao final.   |\n");
        printf("|%78s|\n", "");
        printf("| Este programa é de código aberto e livre dísponivel no "
               "seguinte endereço web |\n");
        printf("|             "
               "<https://github.com/MiraiMindz/banca_de_codigo_uerj>           "
               "  |\n");
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
        printf("[1] - Ementa interativa\n");
        printf("[2] - Fila Linear Estatica\n");
        printf("[0] - Sair\n\n");

        printf("Insira sua Resposta: ");
        scanf("%hhu", &escolha_global);

        switch (escolha_global) {
        case 1:
              printf("Em Progresso...\n");
              break;
        case 2:
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
                    testar_fila_estatica_automatico();
                    break;
                case 2:
                    fila_estatica_menu_interativo();
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
}
