# include "free.h"

void free_arvore(No *raiz)
{
    if (!raiz)
        return;
    free_arvore(raiz->esq);
    free_arvore(raiz->dir);
    free(raiz);
}

static void cleanup_user_dirs(AVL *node)
{
    if (!node)
        return;
    cleanup_user_dirs(node->left);
    cleanup_user_dirs(node->right);
    if (node->user && node->user->name[0] != '\0')
    {
        DIR *d = opendir(node->user->name);
        if (d)
        {
            closedir(d);
            remover_diretorio(node->user->name);
        }
    }
}

void free_everything(Auth *auth)
{
    if (!auth)
        return;
    cleanup_user_dirs(auth->users);
    printf("A sair...\n");
}
