# Anotações

## Coisas para fazer:

- GUI
    - [ ] Tornar GUI responsivo
    - [ ] Transformar as estruturas de dados no GUI interativas:
        - > Por exemplo, vamos supor que queiramos representar uma lista duplamente encadeada, poderiamos exibir uma tabela que representa celulas de memória, visualizar as celulas da lista em uma cor diferente, e quando o usuário clicar em uma delas, marcar a célula anterior e posterior se houverem.
        - > Outro exemplo, ao exibir uma arvore, ao clicar em um nódulo, exibir as informações desse nódulo, como pai (ou mãe), filhos e o dado que guardam.
    - [ ] (talvez) adicionar tema claro/escuro para o GUI

- CLI
    - [ ] fazer com que o paginador (less) aceite as setas em modo imediato
        - seta para cima (subir no texto)
        - seta para baixo (descer no texto)
        - seta para esquerda (pagina/seção anterior)
        - seta para direita (próxima pagina/seção)
        - também permitir que o usuário digite q para sair ou um número para ir para determinada seção.
    - [ ] refatorar a função de exibição de linhas do menu
    - [x] fazer com que o paginador ignore cores na contagem de bytes das strings
    - [x] fazer com que o paginador considere caracteres de multiplos bytes (UTF-8)
    - [x] fazer o paginador processar markdown

- Programa
    - [ ] localizar os códigos e escrever comentários mais descritivos (retroativa)
    - [ ] tornar a raylib estática e parte do programa sem precisar baixar usando o CMAKE
    - [ ] permitir compilação usando GCC
    - [ ] escrever scripts de compilação para windows
    - [ ] produzir binários (executáveis) pré-compilados para execução do programa em qualquer maquina DOS ou UNIX.
    - [ ] implementar as seguintes estruturas de dados:
        - [x] fila linear de tamanho fixo
        - [ ] fila linear de tamanho dinamico
        - [x] pilha linear de tamanho fixo
        - [ ] pilha linear de tamanho dinamico
        - [ ] fila circular
        - [ ] pilha circular
            - > A pilha circular não é muito conhecida ou usada, embora possível
        - [ ] deque de tamanho fixo
        - [ ] deque de tamanho dinamico
        - [ ] fila encadeada
        - [ ] fila duplamente encadeada
        - [ ] pilha encadeada
            - > A pilha encadeada não é muito conhecida ou usada, embora possível
            porque uma pilha encadeada então? simples, para casos de memória fragmentada.
        - [ ] lista de tamanho fixo
        - [ ] lista de tamanho dinamico
        - [ ] lista encadeada
        - [ ] lista duplamente encadeada
            - > a diferença entre uma fila e uma lista é que a lista tem inserção e
            remoção em posições arbitrarias enquanto a fila não.
        - [ ] Tabela de dispersão (tabela hash/hash table)
        - [ ] Conjuntos
        - [ ] árvores
            - [ ] árvores binarias
            - [ ] árvores N-árias
            - [ ] árvores balanceadas
        - [ ] grafo
