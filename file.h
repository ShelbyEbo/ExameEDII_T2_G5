#ifndef FILE_H
#define FILE_H
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct file
{
    int id;
    char *name;
    FILE *filepath;
} File;

typedef struct file_list
{
    File *file;
    struct file_list *next;
} FileList;

FileList *adicionar_ficheiro(FileList *head, int id, const char *name, const char *filepath); 
FileList *remover_ficheiro(FileList *head, int id);
void listar_ficheiros(FileList *head);
File *procurar_por_id(FileList *head, int id);

#endif