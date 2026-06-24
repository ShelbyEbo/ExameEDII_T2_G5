# include "presentation.h"
# include "auth.h"
# include "avl.h"
# include "user.h"

static void clear_input_buffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

static int get_int(const char *prompt, int *value)
{
    int r;

    while (1)
    {
        printf("%s", prompt);
        r = scanf("%d", value);
        if (r == EOF)
        {
            printf("\nA sair...\n");
            return EOF;
        }
        if (r == 1)
            return 1;
        printf("Entrada inválida.\n");
        clear_input_buffer();
    }
}

static int get_string(const char *prompt, char *buf)
{
    int r;

    while (1)
    {
        printf("%s", prompt);
        r = scanf("%99s", buf);
        if (r == EOF)
        {
            printf("\nA sair...\n");
            return EOF;
        }
        if (r == 1)
            return 1;
        printf("Entrada inválida.\n");
        clear_input_buffer();
    }
}

int menu_create_user(Auth *auth)
{
    char name[100];
    int id;

    if (get_int("ID: ", &id) != 1)
        return 0;
    if (get_string("Nome: ", name) != 1)
        return 0;

    if (find_user(auth->users, id))
    {
        printf("Utilizador com ID %d já existe.\n", id);
        return 1;
    }

    User *user = create_user(id, name);
    if (!user)
    {
        printf("Erro ao criar utilizador.\n");
        return 0;
    }
    auth->users = insert_user(auth->users, user);
    printf("Utilizador criado.\n");
    return 1;
}

int menu_remove_user(Auth *auth)
{
    int id;

    if (get_int("ID a remover: ", &id) != 1)
        return 0;

    if (!find_user(auth->users, id))
    {
        printf("Utilizador não encontrado.\n");
        return 1;
    }
    auth->users = delete_user(auth->users, id);
    printf("Utilizador removido.\n");
    return 1;
}

int menu_find_user(Auth *auth)
{
    int id;

    if (get_int("ID a procurar: ", &id) != 1)
        return 0;
    User *user = find_user(auth->users, id);
    if (!user)
        printf("Utilizador não encontrado.\n");
    else
        printf("ID: %d | Nome: %s | Bloqueado: %d\n", user->id, user->name, user->blocked);
    return 1;
}

int menu_block_user(Auth *auth)
{
    int id;

    if (get_int("ID a bloquear: ", &id) != 1)
        return 0;
    User *user = find_user(auth->users, id);
    if (!user)
        printf("Utilizador não encontrado.\n");
    else
    {
        block_user(user);
        printf("Utilizador bloqueado.\n");
    }
    return 1;
}

int menu_unblock_user(Auth *auth)
{
    int id;

    if (get_int("ID a desbloquear: ", &id) != 1)
        return 0;
    User *user = find_user(auth->users, id);
    if (!user)
        printf("Utilizador não encontrado.\n");
    else
    {
        unblock_user(user);
        printf("Utilizador desbloqueado.\n");
    }
    return 1;
}

int menu_login(Auth *auth)
{
    int id;

    if (get_int("ID: ", &id) != 1)
        return 0;
    login(auth, id);
    return 1;
}

void print_menu(Auth *auth)
{
    printf("\n=== Sistema de Partilha de Ficheiros ===\n");
    if (auth->current_user)
        printf("Sessão: %s (ID %d)\n", auth->current_user->name, auth->current_user->id);
    else
        printf("Sessão: nenhuma\n");
    printf("\n-- Utilizadores --\n");
    printf("1. Criar utilizador\n");
    printf("2. Remover utilizador\n");
    printf("3. Procurar utilizador\n");
    printf("4. Listar utilizadores\n");
    printf("5. Bloquear utilizador\n");
    printf("6. Desbloquear utilizador\n");
    printf("33. Menu2\n");
    printf("\n-- Sessão --\n");
    printf("7. Login\n");
    printf("8. Logout\n");
    printf("\n0. Sair\n");
    printf("Opção: ");
}