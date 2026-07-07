#include "auth.h"
#include "user.h"
#include "avl.h"
#include "chat.h"
#include "graph.h"

void login(Auth *auth, int user_id)
{
    User *u = find_user(auth->users, user_id);

    if (!u)
    {
        printf("Utilizador não encontrado.\n");
        return;
    }
    if (u->blocked)
    {
        printf("Utilizador bloqueado.\n");
        return;
    }
    auth->current_user = u;
    printf("Sessão iniciada: %s\n", u->name);
}

void logout(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Nenhuma sessão activa.\n");
        return;
    }
    printf("Sessão terminada: %s\n", auth->current_user->name);
    auth->current_user = NULL;
}

void block_user(User *user)
{
    if (user)
        user->blocked = 1;
}

void unblock_user(User *user)
{
    if (user)
        user->blocked = 0;
}

Auth *init_auth(void)
{
    Auth *auth = malloc(sizeof(Auth));
    if (!auth)
        return NULL;

    auth->users        = NULL;
    auth->current_user = NULL;

    auth->message = malloc(sizeof(MessageQueue));
    if (!auth->message)
    {
        free(auth);
        return NULL;
    }
    queue_init(auth->message);

    auth->chat = malloc(sizeof(ChatHistory));
    if (!auth->chat)
    {
        free(auth->message);
        free(auth);
        return NULL;
    }
    history_init(auth->chat);

    auth->graph = malloc(sizeof(ShareGraph));
    if (!auth->graph)
    {
        free(auth->chat);
        free(auth->message);
        free(auth);
        return NULL;
    }
    graph_init(auth->graph);

    return auth;
}

void destroy_auth(Auth *auth)
{
    if (!auth)
        return;
    avl_destroy(auth->users);
    queue_destroy(auth->message);
    free(auth->message);
    history_destroy(auth->chat);
    free(auth->chat);
    graph_destroy(auth->graph);
    free(auth->graph);
    free(auth);
}