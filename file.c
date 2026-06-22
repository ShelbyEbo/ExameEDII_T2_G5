#include "file.h"

FileList *adicionar_ficheiro(FileList *head, int id, const char *name, FILE *filepath)
{
    FileList *ret = malloc(sizeof(FileList));

    if (!ret)
        return (head);
    ret->file = malloc(sizeof(File));
    if (!ret->file)
    {
        free(ret);
        return (head);
    }
    ret->file->name = malloc(strlen(name) + 1);
    strcpy(ret->file->name, name);
    ret->file->id = id;
    ret->file->filepath = filepath;
    ret->next = head;
    return (ret);
}
FileList *remover_ficheiro(FileList *head, int id)
{
    FileList *curr = head;
    FileList *prev = NULL;

    while (curr && curr->file->id != id)
    {
        prev = curr;
        curr = curr->next;
    }

    if (!curr)
        return (head);

    if (!prev)
        head = curr->next;
    else
        prev->next = curr->next;

    fclose(curr->file->filepath);
    free(curr->file->name);
    free(curr->file);
    free(curr);
    return (head);
}
void listar_ficheiros(FileList *head)
{
    FileList *aux = head;

    while (aux)
    {
        printf("Nome: %s\n", aux->file->name);
        printf("Id: %d\n", aux->file->id);
        aux = aux->next;
    }
}

File *procurar_por_id(FileList *head, int id)
{
    FileList *aux = head;

    while (aux)
    {
        if (aux->file->id == id)
            return (aux->file);

        aux = aux->next;
    }
    return (NULL);
}