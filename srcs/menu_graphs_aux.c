#include "menu_graphs_aux.h"

int graph_registar(Auth *auth)
{
    header("PARTILHAS > Registar Partilha");

    int eof = 0;
    User *s = prompt_user(auth, "ID do remetente: ", &eof);
    if (eof) return 0;
    if (!s)  return 1;

    User *r = prompt_user(auth, "ID do destinatário: ", &eof);
    if (eof) return 0;
    if (!r)  return 1;

    if (!s->files)
    {
        printf("  O utilizador '%s' não tem ficheiros.\n", s->name);
        return 1;
    }

    /* Listar ficheiros do remetente */
    printf("  Ficheiros de %s:\n", s->name);
    listar_ficheiros(s->files);

    int fid;
    if (get_int("  ID do ficheiro a partilhar: ", &fid) != 1)
        return 0;

    File *file = procurar_por_id(s->files, fid);
    if (!file)
    {
        printf("  Ficheiro não encontrado.\n");
        return 1;
    }

    if (graph_add_share(auth->graph, s, r, file) == 0)
        printf("\n  Partilha registada!\n");
    else
        printf("\n  Erro ao registar partilha.\n");
    return 1;
}

int graph_add_user_menu(Auth *auth)
{
    header("PARTILHAS > Adicionar Utilizador ao Grafo");
    int eof = 0;
    User *u = prompt_user(auth, "ID do utilizador: ", &eof);
    if (eof) return 0;
    if (!u)  return 1;

    int idx = graph_add_user(auth->graph, u);
    if (idx >= 0)
        printf("\n  Utilizador '%s' registado no grafo (índice %d).\n", u->name, idx);
    else
        printf("\n  Erro ao registar utilizador.\n");
    return 1;
}

void graph_listar(Auth *auth)
{
    header("PARTILHAS > Grafo de Partilhas");
    if (auth->graph->user_count == 0)
    {
        printf("  Sem dados.\n");
        return;
    }
    graph_print(auth->graph);
}

void graph_users(Auth *auth)
{
    header("PARTILHAS > Utilizadores Registados no Grafo");
    if (auth->graph->user_count == 0)
    {
        printf("  Nenhum utilizador.\n");
        return;
    }
    for (int i = 0; i < auth->graph->user_count; i++)
        printf("  [%d] %s\n", i, auth->graph->users[i]->name);
    printf("\n  Total: %d\n", auth->graph->user_count);
}

int graph_dfs_menu(Auth *auth)
{
    header("PARTILHAS > Pesquisa DFS");
    if (auth->graph->user_count == 0)
    {
        printf("  Sem utilizadores no grafo.\n");
        return 1;
    }
    int eof = 0;
    User *u = prompt_user(auth, "ID do utilizador de partida: ", &eof);
    if (eof) return 0;
    if (!u)  return 1;
    graph_dfs(auth->graph, u);
    return 1;
}

int graph_bfs_menu(Auth *auth)
{
    header("PARTILHAS > Pesquisa BFS");
    if (auth->graph->user_count == 0)
    {
        printf("  Sem utilizadores no grafo.\n");
        return 1;
    }
    int eof = 0;
    User *u = prompt_user(auth, "ID do utilizador de partida: ", &eof);
    if (eof) return 0;
    if (!u)  return 1;
    graph_bfs(auth->graph, u);
    return 1;
}
