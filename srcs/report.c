#include "report.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void count_sent(const ShareGraph *g, int *sent_count)
{
    for (int i = 0; i < g->user_count; i++)
    {
        sent_count[i] = 0;
        Share *cur = g->adj[i];
        while (cur)
        {
            sent_count[i]++;
            cur = cur->next;
        }
    }
}

static void count_received(const ShareGraph *g, int *recv_count)
{
    for (int i = 0; i < g->user_count; i++)
        recv_count[i] = 0;

    for (int i = 0; i < g->user_count; i++)
    {
        Share *cur = g->adj[i];
        while (cur)
        {
            recv_count[cur->to->id]++;
            cur = cur->next;
        }
    }
}

static int cmp_file_count(const void *a, const void *b)
{
    return ((FileCount *)b)->count - ((FileCount *)a)->count;
}

// Partilhas por utilizador
void report_shares_per_user(const ShareGraph *g)
{
    int sent[MAX_USERS], recv[MAX_USERS];
    count_sent(g, sent);
    count_received(g, recv);

    printf("\n+----------------------------------------------+\n");
    printf("|       Partilhas por Utilizador               |\n");
    printf("+----------------------------------------------+\n");
    printf("  %-20s  %8s  %9s\n", "Utilizador", "Enviadas", "Recebidas");
    printf("  %-20s  %8s  %9s\n", "--------------------", "--------", "---------");
    for (int i = 0; i < g->user_count; i++)
    {
        printf("  %-20s  %8d  %9d\n",
               g->users[i]->name, sent[i], recv[i]);
    }
    printf("+----------------------------------------------+\n\n");
}

// Utilizador que mais partilhou
void report_top_sender(const ShareGraph *g)
{
    if (g->user_count == 0)
    {
        printf("[Relatorio] Sem utilizadores registados.\n");
        return;
    }

    int sent[MAX_USERS];
    count_sent(g, sent);

    int best = 0;
    for (int i = 1; i < g->user_count; i++)
    {
        if (sent[i] > sent[best])
            best = i;
    }

    printf("\n+----------------------------------------------+\n");
    printf("|  Utilizador que Mais Partilhou               |\n");
    printf("+----------------------------------------------+\n");
    printf("  %-20s  %d partilha(s) enviada(s)\n",
           g->users[best]->name, sent[best]);
    printf("+----------------------------------------------+\n\n");
}

// Utilizador que mais recebeu
void report_top_receiver(const ShareGraph *g)
{
    if (g->user_count == 0)
    {
        printf("[Relatorio] Sem utilizadores registados.\n");
        return;
    }

    int recv[MAX_USERS];
    count_received(g, recv);

    int best = 0;
    for (int i = 1; i < g->user_count; i++)
    {
        if (recv[i] > recv[best])
            best = i;
    }

    printf("\n+----------------------------------------------+\n");
    printf("|  Utilizador que Mais Recebeu                 |\n");
    printf("+----------------------------------------------+\n");
    printf("  %-20s  %d partilha(s) recebida(s)\n",
           g->users[best]->name, recv[best]);
    printf("+----------------------------------------------+\n\n");
}

// Ficheiros mais partilhados
void report_most_shared_files(const ShareGraph *g)
{
    int max_files = g->total_shares + 1;
    FileCount *files = (FileCount *)calloc(max_files, sizeof(FileCount));
    if (!files)
    {
        printf("  (erro de memoria)\n");
        return;
    }
    int nfiles = 0;

    for (int i = 0; i < g->user_count; i++)
    {
        Share *cur = g->adj[i];
        while (cur)
        {
            int found = -1;
            for (int f = 0; f < nfiles; f++)
            {
                if (strcmp(files[f].file->name, cur->file->name) == 0)
                {
                    found = f;
                    break;
                }
            }
            if (found >= 0)
            {
                files[found].count++;
            }
            else
            {
                strncpy(files[nfiles].file->name, cur->file->name, MAX_FILENAME - 1);
                files[nfiles].file->name[MAX_FILENAME - 1] = '\0';
                files[nfiles].count = 1;
                nfiles++;
            }
            cur = cur->next;
        }
    }

    qsort(files, nfiles, sizeof(FileCount), cmp_file_count);

    printf("\n+----------------------------------------------+\n");
    printf("|  Ficheiros Mais Partilhados                  |\n");
    printf("+----------------------------------------------+\n");
    if (nfiles == 0)
    {
        printf("  (nenhum ficheiro registado)\n");
    }
    else
    {
        printf("  %-30s  %s\n", "Ficheiro", "Vezes");
        printf("  %-30s  %s\n", "------------------------------", "-----");
        for (int f = 0; f < nfiles; f++)
        {
            printf("  %-30s  %d\n", files[f].file->name, files[f].count);
        }
    }
    printf("+----------------------------------------------+\n\n");
    free(files);
}

// Total de membros
void report_total_members(const ShareGraph *g)
{
    printf("\n+----------------------------------------------+\n");
    printf("|  Membros Registados                          |\n");
    printf("+----------------------------------------------+\n");
    printf("  Total de membros  : %d\n", g->user_count);
    printf("  Total de partilhas: %d\n", g->total_shares);
    printf("+----------------------------------------------+\n\n");
}

// Todos os relatorios
void report_all(const ShareGraph *g)
{
    printf("\n\n========== RELATORIO GERAL ==========\n");
    report_total_members(g);
    report_shares_per_user(g);
    report_top_sender(g);
    report_top_receiver(g);
    report_most_shared_files(g);
    printf("=====================================\n\n");
}
