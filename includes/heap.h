#ifndef HEAP_H
#define HEAP_H
#include "file.h"
#define MAX_CHAR 256
#define MAX_CODE 100

typedef struct No 
{
    char caractere;
    int freq;
    struct No *esq;
    struct No *dir;
} No;

typedef struct 
{
    int tamanho;
    int capacidade;
    No **dados;
} MinHeap;

extern char codigos[MAX_CHAR][MAX_CODE];

No* criarNo(char c, int freq);
MinHeap* criarHeap(int capacidade);
void trocar(No **a, No **b);
void heapifyUp(MinHeap *heap, int idx);
void heapifyDown(MinHeap *heap, int idx);
void inserirHeap(MinHeap *heap, No *no);
No* extrairMin(MinHeap *heap);
void gerarCodigos(No *raiz, char codigo[], int topo);
int tamanhoOriginal(char texto[]);
int tamanhoComprimido(char texto[]);
float taxaCompressao(int original, int comprimido);

#endif