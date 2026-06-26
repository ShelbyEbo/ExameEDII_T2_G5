#ifndef PRESENTATION2_H
#define PRESENTATION2_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Auth Auth;

// Menu de Chat
void menu_chat_enviar(Auth *auth);
void menu_chat_receber(Auth *auth);
void menu_chat_historico(Auth *auth);
void menu_chat_historico_user(Auth *auth);
void menu_chat_status(Auth *auth);
void menu_chat(Auth *auth);

// Menu de Partilhas
void menu_graph_registar(Auth *auth);
void menu_graph_add_user(Auth *auth);
void menu_graph_listar(Auth *auth);
void menu_graph_users(Auth *auth);
void menu_graph_dfs(Auth *auth);
void menu_graph_bfs(Auth *auth);
void menu_graph(Auth *auth);

// Menu de Relatorios
void menu_reports(Auth *auth);
int menu2(Auth *auth);

// Demonstracao
void carregar_demo(Auth *auth);

// Free
void limpar_dados(Auth *auth);

#endif