#include "menu_graphs_aux.h"
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
#define MKDIR_GRAPH(path) mkdir(path)
#else
#define MKDIR_GRAPH(path) mkdir(path, 0755)
#endif

static void copy_file_on_disk(const char *src, const char *dst)
{
    FILE *in = fopen(src, "rb");
    if (!in)
        return;
    FILE *out = fopen(dst, "wb");
    if (!out)
    {
        fclose(in);
        return;
    }
    unsigned char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
        fwrite(buf, 1, n, out);
    fclose(in);
    fclose(out);
}

int graph_registar(Auth *auth)
{
    header("PARTILHAS > Registar Partilha");

    User *s = auth->current_user;

    int eof = 0;
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
    {
        printf("\n  Partilha registada!\n");

        /* Copiar ficheiro para o destinatário */
        int res = MKDIR_GRAPH(r->name);
        if (res != 0 && errno != EEXIST)
        {
            printf("  Aviso: não foi possível criar directório do destinatário.\n");
            return 1;
        }

        char src_path[512], dst_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", s->name, file->name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", r->name, file->name);
        copy_file_on_disk(src_path, dst_path);

        int new_id = max_file_id(r->files) + 1;
        r->files = adicionar_ficheiro(r->files, new_id, file->name, fopen(dst_path, "a"));
        printf("  Ficheiro '%s' adicionado aos ficheiros de %s (ID %d).\n",
               file->name, r->name, new_id);
    }
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
