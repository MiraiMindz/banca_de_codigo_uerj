/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */
#ifndef INTERNAL_CLI_H
#define INTERNAL_CLI_H

#include <stdarg.h>
#include <config/config.h>

typedef enum cores_terminal_e {
    PADRAO,
    PRETO_CLARO,
    VERMELHO_CLARO,
    VERDE_CLARO,
    AMARELO_CLARO,
    AZUL_CLARO,
    MAGENTA_CLARO,
    CIANO_CLARO,
    BRANCO_CLARO,
    PRETO_ESCURO,
    VERMELHO_ESCURO,
    VERDE_ESCURO,
    AMARELO_ESCURO,
    AZUL_ESCURO,
    MAGENTA_ESCURO,
    CIANO_ESCURO,
    BRANCO_ESCURO
} cores_terminal_t;

typedef enum alinhamento_textual_e {
    ESQUERDA,
    CENTRO,
    DIREITA
} alinhamento_textual_t;

void cli_definir_cores(cores_terminal_t cor_texto, cores_terminal_t cor_fundo);
void cli_redefinir_cores(void);
void exibir_linha_topo_interface(void);
void exibir_linha_inferior_interface(void);
void exibir_divisor_interface(void);
void exibir_espacador_interface(void);
void exibir_linha_textual_interface(char *formato, alinhamento_textual_t alinhamento, cores_terminal_t cor_texto, cores_terminal_t cor_fundo, ...);
void exibir_opcao_interface(unsigned long numero, char* opcao);
void paginador(char *texto);
void exibir_opcao_configurar_display(unsigned long numero, char *opcao, char* descricao, unsigned char condicional);
void processador_markdown(char* texto);

#endif /* INTERNAL_CLI_H */
