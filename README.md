# Exame EDII — T2 G5

Sistema de compressão e descompressão de ficheiros com codificação de Huffman, desenvolvido em C como projecto de exame da disciplina de Estruturas de Dados II.

## Funcionalidades

- Compressão de ficheiros com codificação de Huffman (`.huff`)
- Descompressão de ficheiros `.huff`
- Sistema de autenticação de utilizadores com árvore AVL
- Fila de mensagens entre utilizadores
- Histórico de conversas
- Grafo de partilha de ficheiros entre utilizadores
- Gestão de ficheiros por utilizador

## Estruturas de Dados

| Estrutura | Uso |
|---|---|
| Árvore AVL | Armazenamento e pesquisa de utilizadores |
| Min-Heap | Construção da árvore de Huffman |
| Árvore de Huffman | Geração de códigos de compressão |
| Fila (Queue) | Sistema de mensagens |
| Lista ligada | Histórico de chat e lista de ficheiros |
| Grafo | Partilha de ficheiros entre utilizadores |

## Compilação

```bash
make
```

Para limpar os ficheiros compilados:

```bash
make fclean
```

## Utilização

```bash
./exame
```

O programa apresenta um menu interactivo com as seguintes opções:

- Autenticação (registo e login)
- Comprimir ficheiro
- Descomprimir ficheiro
- Enviar mensagem
- Ver histórico de chat
- Gerir ficheiros partilhados

Os ficheiros comprimidos são guardados na pasta `compress/` e os descomprimidos em `decompress/`, ambas criadas automaticamente e removidas ao sair do programa.

## Estrutura do Projecto

```
.
├── includes/       # headers (.h)
├── srcs/           # código fonte (.c)
├── obj/            # ficheiros objecto (gerados pelo make)
├── Makefile
└── README.md
```

## Formato do ficheiro .huff

Cada ficheiro comprimido contém:

1. Tamanho original do ficheiro (`long`)
2. Tabela de frequências (`int[256]`)
3. Dados comprimidos em bits

## Autores

Grupo 5 — Turma T2  
Luís Carlos Baptista
Orisa Ebo
Paulo Gaspar
ISPTEC — Estruturas de Dados II
