#include "auth.h"
#include "user.h"
#include "avl.h"

void login(Auth *auth, int user_id)
{
    User *u = find_user(auth->users, user_id);

    if (!u)
    {
        printf("User not found\n");
        return;
    }

    if (u->blocked)
    {
        printf("User is blocked\n");
        return;
    }

    auth->current_user = u;

    printf("Logged in as %s\n", u->name);
}

void logout(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("No user logged in\n");
        return;
    }

    auth->current_user = NULL;

    printf("Logged out\n");
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

Auth *init_auth()
{
    Auth *auth = malloc(sizeof(Auth));
    if (!auth)
        return NULL;
    auth->users = NULL;
    auth->message = NULL;
    auth->chat = NULL;
    auth->graph = NULL;
    auth->current_user = NULL;
    return auth;
}

void destroy_auth(Auth *auth)
{
    if (!auth)
        return;

    free(auth);
}