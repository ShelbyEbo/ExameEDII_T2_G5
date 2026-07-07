#ifndef MENU_GRAPHS_AUX_H
#define MENU_GRAPHS_AUX_H
#include "auth.h"
#include "avl.h"
#include "user.h"
#include "file.h"
#include "menus.h"
#include "graph.h"
#include "menus_aux.h"
#include "menu_chat_aux.h"

int graph_registar(Auth *auth);
int graph_add_user_menu(Auth *auth);
void graph_listar(Auth *auth);
void graph_users(Auth *auth);
int graph_dfs_menu(Auth *auth);
int graph_bfs_menu(Auth *auth);


#endif