#ifndef GRAPH_H
#define GRAPH_H

#include "user.h"
#include "file.h"

#define MAX_USERS 128
#define MAX_FILENAME 256

typedef struct Share
{
    User *from;
    User *to;
    File *file;
    struct Share *next;
} Share;

typedef struct
{
    User *users[MAX_USERS];
    int user_count;

    Share *adj[MAX_USERS];
    int total_shares;
} ShareGraph;

void graph_init(ShareGraph *g);
void graph_destroy(ShareGraph *g);

int graph_add_user(ShareGraph *g, User *user);
int graph_find_user(const ShareGraph *g, const User *user);

int graph_add_share(ShareGraph *g,
                    User *from, User *to,
                    File *file);
void graph_print(const ShareGraph *g);

void graph_dfs(const ShareGraph *g, const User *start_user);
void graph_bfs(const ShareGraph *g, const User *start_user);

#endif
