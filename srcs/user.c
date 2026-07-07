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
    FileList *f = user->files;
    while (f)
    {
        FileList *next = f->next;
        if (f->file)
        {
            if (f->file->filepath)
                fclose(f->file->filepath);
            free(f->file->name);
            free(f->file);
        }
        free(f);
        f = next;
    }
    user->files = NULL;
    free(user);
}