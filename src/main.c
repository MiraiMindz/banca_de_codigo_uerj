/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
/*******************************************************************************
 * Este projeto utiliza C89 (padrão ANSI X3.159-1989) ele consiste em um
 * programa para auxilio na compreensão de estruturas de dado e da linguagem C
 * em geral, contendo uma ementa/apostila interativa, além de uma interface
 * gráfica usando as bibliotecas RayLib e RayGUI.
 *
 * Para compilar este projeto é recomendado ter o compilador LLVM-CLANG com 
 * o sistema de construção (build system) CMAKE, todas as dependencias devem ser
 * gerenciadas automaticamente pelo CMAKE, não sendo necessárias nenhuma outra
 * configuração.
 *
 * Mas para aqueles que quiserem, devem existir versões pré-compiladas para
 * UNIX (Linux), Darwin (MacOS) e MSDOS (Windows).
 ******************************************************************************/

#ifdef _WIN32
    #define clear_screen() system("cls")
#else
    #define clear_screen() system("clear")
#endif

#include <project.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    unsigned char escolha;
    unsigned char loop;
    escolha = 0;
    loop = 0;
    clear_screen();
    do {
        printf("Você gostaria de uma interface de terminal/console ou uma "
               "interface gráfica?\n\n");
        printf("[1] - Terminal/Console\n");
        printf("[2] - Gráfica\n");
        printf("[0] - Sair\n\n");

        printf("Insira sua resposta: ");
        scanf("%hhu", &escolha);

        switch (escolha) {
        case 1:
            clear_screen();
            main_menu();
            /* loop = 1; */
            clear_screen();
            break;
        case 2:
            clear_screen();
            gui_menu();
            /* loop = 1; */
            clear_screen();
            break;
        case 0:
            clear_screen();
            loop = 1;
            clear_screen();
            break;
        default:
            clear_screen();
            printf("Opção Invalida, por favor tente novamente\n\n");
            break;
        }
    } while (loop != 1);
    return 0;
}
