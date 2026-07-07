#include "user.h"

User *create_user(int id, char *name)
{
    User *user = (User *)malloc(sizeof(User));
    if (!user)
        return NULL;

    user->id = id;
    user->blocked = 0;

    strncpy(user->name, name, 99);
    user->name[99] = '\0';

    user->files = NULL;

    return user;
}

void destroy_user(User *user)
{
    if (!user)
        return;
    free(user);
}