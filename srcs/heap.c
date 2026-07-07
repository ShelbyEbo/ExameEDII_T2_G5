#include "heap.h"

char codigos[MAX_CHAR][MAX_CODE];

No* criarNo(char c, int freq)
{
    No *novo = (No*)malloc(sizeof(No));

    novo->caractere = c;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;

    return (novo);
}

MinHeap* criarHeap(int capacidade)
{
    MinHeap *heap = malloc(sizeof(MinHeap));

    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->dados = malloc(capacidade * sizeof(No*));

    return (heap);
}

void trocar(No **a, No **b)
{
    No *temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap *heap, int idx)
{
    while(idx > 0 && heap->dados[idx]->freq < heap->dados[(idx - 1)/2]->freq)
    {
        trocar(&heap->dados[idx], &heap->dados[(idx - 1)/2]);
        idx = (idx - 1)/2;
    }
}

void heapifyDown(MinHeap *heap, int idx)
{
    int menor = idx;

    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if(esq < heap->tamanho && heap->dados[esq]->freq < heap->dados[menor]->freq)
        menor = esq;
    if(dir < heap->tamanho && heap->dados[dir]->freq < heap->dados[menor]->freq)
        menor = dir;
    if(menor != idx)
    {
        trocar(&heap->dados[idx], &heap->dados[menor]);
        heapifyDown(heap, menor);
    }
}

void inserirHeap(MinHeap *heap, No *no)
{
    heap->dados[heap->tamanho] = no;
    heapifyUp(heap, heap->tamanho);
    heap->tamanho++;
}

No* extrairMin(MinHeap *heap)
{
    No *menor = heap->dados[0];

    heap->dados[0] = heap->dados[heap->tamanho - 1];
    heap->tamanho--;
    heapifyDown(heap, 0);

    return (menor);
}

void gerarCodigos(No *raiz, char codigo[], int topo)
{
    if(raiz->esq)
    {
        codigo[topo] = '0';
        gerarCodigos(raiz->esq, codigo, topo + 1);
    }
    if(raiz->dir)
    {
        codigo[topo] = '1';
        gerarCodigos(raiz->dir, codigo, topo + 1);
    }
    if(!raiz->esq && !raiz->dir)
    {
        codigo[topo] = '\0';

        strcpy(codigos[(unsigned char)raiz->caractere], codigo);
        printf("%c -> %s\n", raiz->caractere, codigo);
    }
}

int tamanhoOriginal(char texto[])
{
    return (strlen(texto) * 8);
}

int tamanhoComprimido(char texto[])
{
    int total = 0;

    for(int i = 0; texto[i]; i++)
        total += strlen(codigos[(unsigned char)texto[i]]);
    return (total);
}

float taxaCompressao(int original, int comprimido)
{
    return ((1.0f - (float)comprimido / original) * 100);
}
