#ifndef STORAGE_H
#define STORAGE_H
#include "file.h"

typedef struct 
{
    char textoOriginal[1000];
    char comprimido[5000];
    int tamanhoOriginal;
    int tamanhoComprimido;
} Registro;

void salvarRegistro(Registro r);
void carregarRegistros();

#endif