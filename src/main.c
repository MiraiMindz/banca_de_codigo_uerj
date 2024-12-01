/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Este projeto utiliza C89 (padrão ANSI X3.159-1989)
 *******************************************************************************/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    unsigned char escolha;
    unsigned char loop;
    escolha = 0;
    loop = 0;
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    do {
        printf("Você gostaria de uma interface de terminal/console ou uma interface gráfica?\n\n");
        printf("[1] - Terminal/Console\n");
        printf("[2] - Gráfica\n");
        printf("[0] - Sair\n\n");

        printf("Insira sua resposta: ");
        scanf("%hhu", &escolha);

        switch (escolha) {
            case 1:
                #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
                main_menu();
                loop = 1;
                break;
            case 2:
            #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
                gui_menu();
                loop = 1;
                break;
            case 0:
            #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
                loop = 1;
                break;
            default:
            #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
                printf("Opção Invalida, por favor tente novamente\n\n");
                break;
        }
    } while (loop != 1);
    return 0;
}
