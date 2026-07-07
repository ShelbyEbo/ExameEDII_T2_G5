#ifndef MENUS_H
# define MENUS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <errno.h>
#ifdef _WIN32
 # include <direct.h>
#else
 #include <sys/stat.h>
 #include <sys/types.h>
#endif
# include "free.h"

typedef struct Auth Auth;

void print_menu(Auth *auth);
int  menu_create_user(Auth *auth);
int  menu_remove_user(Auth *auth);
int  menu_find_user(Auth *auth);
int  menu_block_user(Auth *auth);
int  menu_unblock_user(Auth *auth);
int  menu_login(Auth *auth);
int  menu_add_file(Auth *auth);
int  menu_remove_file(Auth *auth);
int  menu_list_files(Auth *auth);
int  menu_huffman_compress(Auth *auth);
int  menu_huffman_compress_file(Auth *auth);
int  menu_huffman_decompress_file(Auth *auth);
int  menu_chat(Auth *auth);
int  menu_graph(Auth *auth);
int  menu_reports(Auth *auth);

#endif
