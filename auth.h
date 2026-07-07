#ifndef AUTH_H
#define AUTH_H

#include <stdlib.h>
#include <stdio.h>

#include "chat.h"
#include "graph.h"

typedef struct AVLNode AVL;
typedef struct User User;

typedef struct Auth
{
    AVL *users;
    MessageQueue *message;
    ChatHistory *chat;
    ShareGraph *graph;
    User *current_user;
} Auth;

Auth *init_auth();
void logout(Auth *auth);
void block_user(User *user);
void destroy_auth(Auth *auth);
void unblock_user(User *user);
void login(Auth *auth, int user_id);

#endif