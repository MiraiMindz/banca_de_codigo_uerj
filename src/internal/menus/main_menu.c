/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */

#include "estruturas/fila_dinamica_linear_fixa/menu.h"
#include "estruturas/pilha_dinamica_linear_fixa/menu.h"
#include "internal/cli.h"
#include "internal/menus.h"

#include <stdio.h>
#include <stdlib.h>

void main_menu(void) {
    unsigned char escolha_global = 0;
    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        /* Sim um comentário no meio do código, olha... eu SEI que isso está
         * consideravelmente ilegível, mas eu tenho boas justificativas, eu juro
         *
         * 1. meu formatador padrão está configurado para "imitar" códigos de
         * de 1989 ou antes, então ele automaticamente quebra os printf's para
         * caberem dentro de 80 caracteres, além de posicionar as diretrizes de
         * pré-processador na coluna 0 (inicio).
         * 2. eu tinha escrito uma evolução do printf para cores, mas ela só
         * funcionava bem mesmo no linux, no windows era uma confusão porque
         * eu tinha que fazer parsing duplo e calcular espaços e tudo mais.
         * além de que, pela ausencia da função vsnprintf em C89, eu tive que
         * implementar a minha própria função e ela não era 100% (o calculo do
         * buffer inicial não era exato, o que ocasionaria erros em determinados
         * cenários), enfim uma super-engenharia de código que me levou a noite
         * inteira e 380 linhas de código.
         *
         * Tendo isso em vista o que eu fiz? deletei toda a bagunça que eu tinha
         * feito e montei uma API mais simplificada que é compativel com o modo
         * de operação do Windows, basicamente eu tenho uma função que alterna
         * a cor de exibição do terminal e outra que volta para a cor padrão.
         *
         * Então o código fica meio que assim:
         *  - troca de cor
         *  - printa
         *  - reinicia a cor
         *  - printa
         *  - troca a cor
         *  - printa
         *  - reinicia a cor
         *
         * enfim, você entendeu; por conta disso o codigo ficou mais
         * verticalmente extenso e os printf's estão todos quebrados em parte
         * para colorizar do jeito certo as coisas.
         *
         * Eu precisava fazer isso? Não.
         * Eu queria e podia? Sim.
         **/
        tc_set_color(DARK_WHITE, RESET);
        printf("/--------------------------------------------------------------"
               "----------------\\\n");
        tc_reset_color();
        printf("|          ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("Banco de Código da Universidade do Estado do Rio de Janeiro");
        tc_reset_color();
        printf("         |\n");
        printf("|%78s|\n", "");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("Este programa tem o objetivo auxiliar na compreensão de "
               "estruturas de dado");
        tc_reset_color();
        printf("   |\n");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("usando a linguagem C (padrão ANSI X3.159-1989) de programação, "
               "quaisquer");
        tc_reset_color();
        printf("     |\n");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("dúvidas contate o professor ou mande um email ou mensagem para "
               "o contato ao");
        tc_reset_color();
        printf("  |\n");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("final, desta tela.");
        tc_reset_color();
        printf("%59s|\n", "");
        printf("|%78s|\n", "");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("Este programa é de código aberto e livre dísponivel no "
               "seguinte endereço web");
        tc_reset_color();
        printf(" |\n");
        printf("|%13s", "");
        tc_set_color(LIGHT_BLUE, RESET);
        printf("<https://github.com/MiraiMindz/banca_de_codigo_uerj>");
        tc_reset_color();
        printf("%13s|\n", "");
        printf("|%78s|\n", "");
        printf("|--------------------------------------------------------------"
               "----------------|\n");
        printf("|%78s|\n", "");
        printf("| ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("Escrito por Bruno C. Barreto (vulgo Mirai)");
        tc_reset_color();
        printf("%35s|\n", "");
        printf("|   ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("- Email de Contato: barreto2contato@gmail.com");
        tc_reset_color();
        printf("%30s|\n", "");
        printf("|   ");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("- Telefone Celular de Contato: +55 (21) 97157-0580");
        tc_reset_color();
        printf("%25s|\n", "");
        printf("|%78s|\n", "");
        printf("\\-------------------------------------------------------------"
               "-----------------/\n\n");
               
        printf("[");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("1\n");
        printf("] - Ementa interativa\n");
        printf("[");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("2");
        printf("] - Fila linear de tamanho fixo\n");
        printf("[");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("3");
        printf("] - Pilha linear de tamanho fixo\n");
        printf("[");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("9");
        printf("] - Ajuda\n");
        printf("[");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("0");
        printf("] - Sair\n\n");

        printf("Digite os numeros entre [");
        tc_set_color(LIGHT_WHITE, RESET);
        printf("colchetes"); 
        tc_reset_color();
        printf("] para se movimentar na aplicação e pressione\n");
        printf("ENTER para enviar sua resposta.\n\n");

        printf("Insira sua Resposta: ");
        scanf("%hhu", &escolha_global);

        switch (escolha_global) {
        case 1:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Em Progresso...\n");
            break;
        case 2:
            fila_fixa_menu_interativo();
            break;
        case 3:
            testar_pilha_fixa_menu_interativo();
            break;
        case 9:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printf("Digite os numeros entre [colchetes] para se movimentar na aplicação e pressione ENTER para enviar sua resposta.\n");
        case 0:
            tc_set_color(LIGHT_WHITE, RESET);
            printf("Saindo...\n");
            tc_reset_color();
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
