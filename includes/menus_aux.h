#ifndef MENUS_AUX_H
#define MENUS_AUX_H
#include "menus.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void clear_input_buffer(void);
int get_int(const char *prompt, int *value);
int get_line(const char *prompt, char *buf, int size);
void separator(void);
void header(const char *title);
void pause_enter(void);
void clear_screen(void);
void remover_diretorio(const char *caminho);

#endif