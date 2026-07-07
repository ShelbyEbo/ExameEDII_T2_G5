#include "main.h"

int main(void)
{
    Auth *auth = init_auth();

    if (!auth)
    {
        printf("Erro ao inicializar o sistema.\n");
        return 1;
    }

    int option;
    int running = 1;
    do
    {
        int r;

        print_menu(auth);
        while (1)
        {
            r = scanf("%d", &option);
            if (r == EOF)
            {
                printf("\nA sair...\n");
                running = 0;
                break;
            }
            if (r != 1)
            {
                int ch;

                printf("Entrada inválida.\n");
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
                printf("Opção: ");
            }
            else
            {
                break;
            }
        }
        if (!running)
            break;
        switch (option)
        {
        case 1:
            if (!menu_create_user(auth))
                running = 0;
            break;
        case 2:
            if (!menu_remove_user(auth))
                running = 0;
            break;
        case 3:
            if (!menu_find_user(auth))
                running = 0;
            break;
        case 4:
            list_users(auth->users);
            break;
        case 5:
            if (!menu_block_user(auth))
                running = 0;
            break;
        case 6:
            if (!menu_unblock_user(auth))
                running = 0;
            break;
        case 7:
            if (!menu_login(auth))
                running = 0;
            break;
        case 9:
            logout(auth);
            break;
        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (option != 0 && running);
    destroy_auth(auth);
    return 0;
}