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
            if (r == 1)
            {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
                break;
            }
            {
                int ch;
                printf("Entrada inválida.\n");
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
            printf("Opção: ");
        }

        if (!running)
            break;

        switch (option)
        {
            case  1: if (!menu_create_user(auth))  running = 0; break;
            case  2: if (!menu_remove_user(auth))  running = 0; break;
            case  3: if (!menu_find_user(auth))    running = 0; break;
            case  4: list_users(auth->users);                   break;
            case  5: if (!menu_block_user(auth))   running = 0; break;
            case  6: if (!menu_unblock_user(auth)) running = 0; break;
            case  7: if (!menu_login(auth))        running = 0; break;
            case  8: logout(auth);                              break;
            case  9: if (!menu_add_file(auth))     running = 0; break;
            case 10: if (!menu_remove_file(auth))  running = 0; break;
            case 11: if (!menu_list_files(auth))   running = 0; break;
            case 12: if (!menu_huffman_compress_file(auth)) running = 0; break;
            case 13: if (!menu_huffman_decompress_file(auth)) running = 0; break;
            case 14: if (!menu_chat(auth))         running = 0; break;
            case 15: if (!menu_graph(auth))        running = 0; break;
            case 16: if (!menu_reports(auth))      running = 0; break;
            case  0: free_everything(auth); break;
            default: printf("Opção inválida.\n");               break;
        }
    } while (option != 0 && running);

    destroy_auth(auth);
    return 0;
}