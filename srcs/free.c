# include "free.h"

void free_everything(Auth *auth)
{
    if (!auth)
        return;
    remover_diretorio("compress");
    remover_diretorio("decompress");
    printf("A sair...\n");
}
