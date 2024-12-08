/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */

#include <estruturas/fila_dinamica_linear_fixa/menu.h>
#include <estruturas/pilha_dinamica_linear_fixa/menu.h>
#include <internal/cli.h>
#include <internal/ementa_interativa/ementa_interativa.h>
#include <internal/menus.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

void main_menu(void) {
    unsigned char escolha_global = 0;
    limpar_tela();
    do {
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
         *
         * TALVEZ após eu analisar o uso dessa colorização, se eu perceber um
         * padrão, eu abstraio para uma função utilitaria para facilitar a
         * leitura do código, mas por ora, é manual mesmo.
         *
         * Bruno do futuro aqui, eu abstrai parte das repetições, mas alguns
         * elementos ainda estão manualmente colorizados.
         **/

        exibir_linha_topo_interface();
        exibir_linha_textual_interface(
            "Banco de Código da Universidade do Estado do Rio de Janeiro",
            CENTRO, BRANCO_CLARO, PRETO_ESCURO);
        exibir_espacador_interface();
        exibir_linha_textual_interface("Este programa tem o objetivo auxiliar na "
                             "compreensão de estruturas de dados",
                             ESQUERDA, BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface("usando a linguagem C (padrão ANSI X3.159-1989) "
                             "de programação, quaisquer",
                             ESQUERDA, BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface("dúvidas contate o professor ou mande um email ou "
                             "mensagem para o contato ao",
                             ESQUERDA, BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface("final, desta tela.", ESQUERDA, BRANCO_CLARO,
                             PRETO_ESCURO);
        exibir_espacador_interface();
        exibir_linha_textual_interface("Este programa é de código aberto e livre "
                             "dísponivel no seguinte endereço web",
                             CENTRO, BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface(
            "<https://github.com/MiraiMindz/banca_de_codigo_uerj>", CENTRO,
            AZUL_CLARO, PRETO_ESCURO);
        exibir_espacador_interface();
        exibir_divisor_interface();
        exibir_espacador_interface();
        exibir_linha_textual_interface("Escrito por Bruno C. Barreto (vulgo Mirai)", ESQUERDA,
                             BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface("- Email de Contato: barreto2contato@gmail.com",
                             ESQUERDA, BRANCO_CLARO, PRETO_ESCURO);
        exibir_linha_textual_interface(
            "- Telefone Celular de Contato: +55 (21) 97157-0580", ESQUERDA,
            BRANCO_CLARO, PRETO_ESCURO);
        exibir_espacador_interface();
        exibir_linha_inferior_interface();

        exibir_opcao_interface(1, "Ementa interativa");
        exibir_opcao_interface(2, "Filas");
        exibir_opcao_interface(3, "Pilhas");
        exibir_opcao_interface(4, "Listas");
        exibir_opcao_interface(5, "Tabelas");
        exibir_opcao_interface(6, "Grafos (Arvores)");
        exibir_opcao_interface(0, "Sair");
        printf("\n");

        printf("Digite os numeros entre [");
        cli_definir_cores(BRANCO_CLARO, PADRAO);
        printf("colchetes");
        cli_redefinir_cores();
        printf("] para se movimentar na aplicação e pressione\n");
        printf("ENTER para enviar sua resposta.\n\n");

        printf("Insira sua Resposta: ");
        scanf("%hhu", &escolha_global);

        switch (escolha_global) {
        case 1:
            limpar_tela();
            ementa_interativa();
            break;
        case 2:
            limpar_tela();
            fila_fixa_menu_interativo();
            limpar_tela();
            break;
        case 3:
            limpar_tela();
            testar_pilha_fixa_menu_interativo();
            limpar_tela();
            break;
        case 9:
            limpar_tela();

            printf("Digite os numeros entre [");
            cli_definir_cores(BRANCO_CLARO, PADRAO);
            printf("colchetes");
            cli_redefinir_cores();
            printf("] para se movimentar na aplicação e pressione\n");
            printf("ENTER para enviar sua resposta.\n\n");
        case 0:
            limpar_tela();
            cli_definir_cores(BRANCO_CLARO, PADRAO);
            printf("Saindo...\n");
            cli_redefinir_cores();
            break;
        default:
            limpar_tela();
            cli_definir_cores(VERMELHO_CLARO, PADRAO);
            printf("Opção Invalida, favor tentar novamente.\n\n");
            cli_redefinir_cores();
            break;
        }
    } while (escolha_global != 0);
}
