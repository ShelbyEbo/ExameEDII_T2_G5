# include "free.h"

void free_arvore(No *raiz)
{
    if (!raiz)
        return;
    free_arvore(raiz->esq);
    free_arvore(raiz->dir);
    free(raiz);
}

void free_everything(Auth *auth)
{
    if (!auth)
        return;
    remover_diretorio("compress");
    remover_diretorio("decompress");
    printf("A sair...\n");
}
