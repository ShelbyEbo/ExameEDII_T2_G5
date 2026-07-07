#ifndef AVL_H
# define AVL_H

# include <stdlib.h>
# include <stdio.h>

typedef struct User User;

typedef struct AVLNode
{
    User            *user;
    int              height;
    struct AVLNode  *left;
    struct AVLNode  *right;
} AVL;

AVL  *insert_user(AVL *root, User *user);
AVL  *delete_user(AVL *root, int id);
User *find_user(AVL *root, int id);
void  list_users(AVL *root);
void  avl_destroy(AVL *root);

#endif