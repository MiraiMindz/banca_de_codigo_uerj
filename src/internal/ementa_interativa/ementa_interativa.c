/* -*- coding: utf-8 -*- */
/* Encoding: UTF-8 */
/* META-DADOS DO ARQUIVO DE CÓDIGO, NÃO MODIFICAR POR FAVOR */

#include <internal/cli.h>
#include <internal/ementa_interativa/ementa_interativa.h>
#include <stdlib.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

void ementa_interativa(void) {
    limpar_tela();
    paginador(
        "# cabeçalho\n"
        "\n"
        "**negrito**\n"
        "*italico*\n"
        "`inline code` *italico* **negrito**\n"
        "**negrito** `inline code` *italico*\n"
        "*italico* **negrito** `inline code`\n"
        "**code without a newline** "
        "*This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart*\n"
        "**This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart**\n"
        "`This is a text with a really big line that spans across more than 80 characteres=that=really=should=never=be split apart`\n"
        "- a\n"
        "- b\n"
        "  - c\n"
        "  - d\n"
        "  - e\n"
        "    - e\n"
        "  - g\n"
        "> quote\n"
        "> quote\n"
        "> quote\n"
        "> quote\n"
        "```c\n"
        "#include <stdio.h>\n"
        "\n"
        "/* comentario */\n"
        "int main(void) {\n"
        "\tprintf(\"Hello World\\n\");\n"
        "\n"
        "\treturn 0;\n"
        "}\n"
        "```\n"
        "\n"
    );
    limpar_tela();
}

void indice(void) { limpar_tela(); }

void introducao(void) { limpar_tela(); }
