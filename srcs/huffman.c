#include "huffman.h"

No* construirArvore(int freq[])
{
    MinHeap *heap = criarHeap(MAX_CHAR);

    for(int i = 0; i < MAX_CHAR; i++)
    {
        if(freq[i] > 0)
            inserirHeap( heap, criarNo((char)i, freq[i]));
    }
    while(heap->tamanho > 1)
    {
        No *esq = extrairMin(heap);
        No *dir = extrairMin(heap);
        No *pai = criarNo('\0', esq->freq + dir->freq);
        pai->esq = esq;
        pai->dir = dir;
        inserirHeap(heap, pai);
    }
    No *raiz = extrairMin(heap);
    free(heap->dados);
    free(heap);
    return (raiz);
}

void comprimir(char texto[], char saida[])
{
    saida[0] = '\0';

    for(int i = 0; texto[i]; i++)
        strcat(saida, codigos[(unsigned char)texto[i]]);
}

void descomprimir(No *raiz, char bits[])
{
    No *atual = raiz;

    printf("\nTexto recuperado:\n");
    for(int i = 0; bits[i]; i++)
    {
        if(bits[i] == '0')
            atual = atual->esq;
        else
            atual = atual->dir;

        if(!atual->esq && !atual->dir)
        {
            printf("%c", atual->caractere);
            atual = raiz;
        }
    }
    printf("\n");
}
