#include "avl.h"
#include "user.h"

static int height(AVL *n)
{
    if (!n)
        return 0;
    return n->height;
}

static int get_balance(AVL *n)
{
    if (!n)
        return 0;
    return height(n->left) - height(n->right);
}

static int max(int a, int b)
{
    return (a > b) ? a : b;
}

static void update_height(AVL *n)
{
    if (n)
        n->height = 1 + max(height(n->left), height(n->right));
}

static AVL *right_rotate(AVL *y)
{
    AVL *x = y->left;
    AVL *T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

static AVL *left_rotate(AVL *x)
{
    AVL *y = x->right;
    AVL *T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

AVL *insert_user(AVL *node, User *user)
{
    if (!node)
    {
        AVL *new_node = malloc(sizeof(AVL));
        if (!new_node)
            return NULL;
        new_node->user = user;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 1;
        return new_node;
    }

    if (user->id < node->user->id)
        node->left = insert_user(node->left, user);
    else if (user->id > node->user->id)
        node->right = insert_user(node->right, user);
    else
        return node;

    update_height(node);

    int balance = get_balance(node);

    if (balance > 1 && user->id < node->left->user->id)
        return right_rotate(node);

    if (balance < -1 && user->id > node->right->user->id)
        return left_rotate(node);

    if (balance > 1 && user->id > node->left->user->id)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && user->id < node->right->user->id)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

User *find_user(AVL *node, int id)
{
    if (!node)
        return NULL;

    if (id == node->user->id)
        return node->user;

    if (id < node->user->id)
        return find_user(node->left, id);

    return find_user(node->right, id);
}

void list_users(AVL *node)
{
    if (!node)
        return;

    list_users(node->left);

    printf("ID: %d | Name: %s | Blocked: %d\n",
        node->user->id,
        node->user->name,
        node->user->blocked);

    list_users(node->right);
}

static AVL *min_value_node(AVL *node)
{
    AVL *current = node;

    while (current->left)
        current = current->left;

    return current;
}

static AVL *delete_node_only(AVL *root, int id)
{
    if (!root)
        return NULL;
    if (id < root->user->id)
        root->left = delete_node_only(root->left, id);
    else if (id > root->user->id)
        root->right = delete_node_only(root->right, id);
    else
    {
        AVL *temp = root->left ? root->left : root->right;
        free(root);
        return temp;
    }

    update_height(root);

    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);
    if (balance > 1 && get_balance(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    if (balance < -1 && get_balance(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

AVL *delete_user(AVL *root, int id)
{
    if (!root)
        return NULL;
    if (id < root->user->id)
        root->left = delete_user(root->left, id);
    else if (id > root->user->id)
        root->right = delete_user(root->right, id);
    else
    {
        if (!root->left || !root->right)
        {
            AVL *temp = root->left ? root->left : root->right;

            if (!temp)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            destroy_user(temp->user);
            free(temp);
        }
        else
        {
            AVL *temp = min_value_node(root->right);

            destroy_user(root->user);
            root->user = temp->user;

            root->right = delete_node_only(root->right, temp->user->id);
        }
    }

    if (!root)
        return NULL;

    update_height(root);

    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);
    if (balance > 1 && get_balance(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    if (balance < -1 && get_balance(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void avl_destroy(AVL *root)
{
    if (!root)
        return;
    avl_destroy(root->left);
    avl_destroy(root->right);
    destroy_user(root->user);
    free(root);
}