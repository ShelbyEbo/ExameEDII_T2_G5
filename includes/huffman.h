#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "file.h"
#include "heap.h"
#include <sys/stat.h>
#include <errno.h>

No* construirArvore(int freq[]);
void comprimir(char texto[], char saida[]);
void descomprimir(No *raiz, char bits[]);

#endif