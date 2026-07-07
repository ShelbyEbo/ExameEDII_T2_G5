#ifndef AUTH_H
# define AUTH_H

# include <stdlib.h>
# include <stdio.h>

typedef struct AVLNode     AVL;
typedef struct User        User;
typedef struct MessageQueue MessageQueue;
typedef struct ChatHistory  ChatHistory;
typedef struct ShareGraph   ShareGraph;

typedef struct Auth
{
    AVL          *users;
    User         *current_user;
    MessageQueue *message;
    ChatHistory  *chat;
    ShareGraph   *graph;
} Auth;

Auth *init_auth(void);
void  logout(Auth *auth);
void  block_user(User *user);
void  destroy_auth(Auth *auth);
void  unblock_user(User *user);
void  login(Auth *auth, int user_id);

#endif