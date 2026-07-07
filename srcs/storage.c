#include "storage.h"

/*void salvarUsuario(Usuario u)
{
    FILE *f = fopen("usuarios.txt", "a");

    if(f == NULL) return;

    fprintf(f, "%s\n", u.nome);

    fclose(f);
}
    */

void salvarRegistro(Registro r)
{
    FILE *f = fopen("dados.txt", "a");

    if(f == NULL) 
        return ;
    fprintf(f, "ORIGINAL:%s\n", r.textoOriginal);
    fprintf(f, "COMP:%s\n", r.comprimido);
    fprintf(f, "ORIG_SIZE:%d\n", r.tamanhoOriginal);
    fprintf(f, "COMP_SIZE:%d\n", r.tamanhoComprimido);
    fprintf(f, "----\n");
    fclose(f);
}
/*
void carregarUsuarios()
{
    FILE *f = fopen("usuarios.txt", "r");

    if(f == NULL) return;

    char nome[50];

    printf("Usuarios:\n");

    while(fgets(nome, sizeof(nome), f))
    {
        printf("%s", nome);
    }

    fclose(f);
}
    */

void carregarRegistros()
{
    FILE *f = fopen("dados.txt", "r");

    if(f == NULL)
        return;

    char linha[200];
    printf("\nRegistros:\n");

    while(fgets(linha, sizeof(linha), f))
        printf("%s", linha);
    fclose(f);
}