#define _POSIX_C_SOURCE 200809L
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INICIALIZAÇÃO / DESTRUIÇÃO
void graph_init(ShareGraph *g)
{
    g->user_count = 0;
    g->total_shares = 0;
    for (int i = 0; i < MAX_USERS; i++)
    {
        g->users[i] = NULL;
        g->adj[i] = NULL;
    }
}

void graph_destroy(ShareGraph *g)
{
    for (int i = 0; i < g->user_count; i++)
    {
        Share *cur = g->adj[i];
        while (cur)
        {
            Share *tmp = cur->next;
            free(cur);
            cur = tmp;
        }
        g->adj[i] = NULL;
        free(g->users[i]);
        g->users[i] = NULL;
    }
    g->user_count = 0;
    g->total_shares = 0;
}

// GESTÃO DE UTILIZADORES
int graph_find_user(const ShareGraph *g, const User *user)
{
    for (int i = 0; i < g->user_count; i++)
    {
        if (g->users[i] == user)
            return i;
    }
    return -1;
}

int graph_add_user(ShareGraph *g, User *user)
{
    int idx = graph_find_user(g, user);
    if (idx >= 0)
        return idx;

    if (g->user_count >= MAX_USERS)
    {
        fprintf(stderr, "[Graph] Limite de utilizadores atingido.\n");
        return -1;
    }

    idx = g->user_count++;
    g->users[idx] = user;
    g->adj[idx] = NULL;
    return idx;
}

// REGISTO DE PARTILHAS
int graph_add_share(ShareGraph *g,
                    User *from, User *to,
                    File *file)
{
    int from_id = graph_add_user(g, from);
    int to_id = graph_add_user(g, to);
    if (from_id < 0 || to_id < 0)
        return -1;

    if (to->blocked || from->blocked)
    {
        fprintf(stderr, "[Graph] Um dos utilizadores está bloqueado.\n");
        return -1;
    }

    Share *s = (Share *)malloc(sizeof(Share));
    if (!s)
        return -1;

    s->from = from;
    s->to = to;
    s->file = (File *)malloc(sizeof(File));
    if (!s->file)
    {
        free(s);
        return -1;
    }
    s->file->name = strdup(file->name);

    s->next = g->adj[from_id];
    g->adj[from_id] = s;

    g->total_shares++;
    printf("[Graph] Partilha registada: %s -> %s (%s)\n", from->name, to->name, file->name);
    return 0;
}

// IMPRESSÃO DO GRAFO
void graph_print(const ShareGraph *g)
{
    printf("\n+----------------------------------------------+\n");
    printf("|       Grafo de Partilhas                     |\n");
    printf("+----------------------------------------------+\n");
    for (int i = 0; i < g->user_count; i++)
    {
        printf("  %s:\n", g->users[i]->name);
        Share *cur = g->adj[i];
        if (!cur)
        {
            printf("    (sem partilhas)\n");
            continue;
        }
        while (cur)
        {
            printf("    -> %s  [%s]\n",
                   g->users[cur->to->id]->name, cur->file->name);
            cur = cur->next;
        }
    }
    printf("+----------------------------------------------+\n\n");
}

// DFS  (Depth-First Search)
static void dfs_visit(const ShareGraph *g, int v, int *visited, int depth)
{
    visited[v] = 1;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("└─ %s\n", g->users[v]->name);

    Share *cur = g->adj[v];
    while (cur)
    {
        if (!visited[cur->to->id])
            dfs_visit(g, cur->to->id, visited, depth + 1);
        cur = cur->next;
    }
}

void graph_dfs(const ShareGraph *g, const User *start_user)
{
    int start = graph_find_user(g, start_user);
    if (start < 0)
    {
        printf("[DFS] Utilizador '%s' não encontrado.\n", start_user->name);
        return;
    }

    int visited[MAX_USERS] = {0};
    printf("\n+----------------------------------------------+\n");
    printf("|       DFS a partir de '%s'               |\n", start_user->name);
    printf("+----------------------------------------------+\n");
    dfs_visit(g, start, visited, 0);

    /* visita componentes desconexos se existirem */
    for (int i = 0; i < g->user_count; i++)
    {
        if (!visited[i])
            dfs_visit(g, i, visited, 0);
    }
    printf("+----------------------------------------------+\n\n");
}

// BFS  (Breadth-First Search)
void graph_bfs(const ShareGraph *g, const User *start_user)
{
    int start = graph_find_user(g, start_user);
    if (start < 0)
    {
        printf("[BFS] Utilizador '%s' não encontrado.\n", start_user->name);
        return;
    }

    int visited[MAX_USERS] = {0};
    int queue[MAX_USERS];
    int head = 0, tail = 0;

    visited[start] = 1;
    queue[tail++] = start;

    printf("\n+----------------------------------------------+\n");
    printf("|       BFS a partir de '%s'               |\n", start_user->name);
    printf("+----------------------------------------------+\n");

    while (head < tail)
    {
        int v = queue[head++];
        printf("  Visita: %s\n", g->users[v]->name);

        Share *cur = g->adj[v];
        while (cur)
        {
            if (!visited[cur->to->id])
            {
                visited[cur->to->id] = 1;
                queue[tail++] = cur->to->id;
            }
            cur = cur->next;
        }
    }

    for (int i = 0; i < g->user_count; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            queue[tail++] = i;
            printf("  [Componente separado] Visita: %s\n", g->users[i]->name);
            while (head < tail)
            {
                int v = queue[head++];
                Share *cur = g->adj[v];
                while (cur)
                {
                    if (!visited[cur->to->id])
                    {
                        visited[cur->to->id] = 1;
                        queue[tail++] = cur->to->id;
                        printf("  Visita: %s\n", g->users[cur->to->id]->name);
                    }
                    cur = cur->next;
                }
            }
        }
    }
    printf("+----------------------------------------------+\n\n");
}
