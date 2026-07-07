#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <string.h>
#include "file.h"

typedef struct User
{
<<<<<<< HEAD:user.h
    int id;
    int blocked;
    char name[100];
    FileList *files;
=======
    int     id;
    int     blocked;
    char    name[100];
    FileList    *files;
>>>>>>> 8b4f1014795bc3245c8d0ce4b8dc317e1135bbdf:includes/user.h
    // Message *inbox;
    // Share   *shares;
} User;

User *create_user(int id, char *name);
void destroy_user(User *user);

#endif