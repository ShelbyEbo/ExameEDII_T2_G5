#include "menus.h"
#include "auth.h"
#include "avl.h"
#include "user.h"
#include "file.h"
#include "menu_chat_aux.h"
#include "menu_graphs_aux.h"
#include "menus_aux.h"
#include "heap.h"
#include "huffman.h"
#include "storage.h"
#include "chat.h"
#include "graph.h"
#include "report.h"

#ifdef _WIN32
#define MKDIR(path) mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

int menu_create_user(Auth *auth)
{
    int id;
    char name[100];

    if (get_int("ID: ", &id) != 1)
        return 0;
    if (get_line("Nome: ", name, sizeof(name)) != 1)
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
    printf("Utilizador '%s' criado.\n", name);
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
    if (auth->current_user && auth->current_user->id == id)
    {
        printf("Não é possível remover o utilizador com sessão activa.\n");
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
        printf("ID: %d | Nome: %s | Bloqueado: %s\n",
               user->id, user->name, user->blocked ? "Sim" : "Não");
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

int menu_add_file(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    int id;
    char name[256];
    int resultado = MKDIR(auth->current_user->name);

    if (resultado != 0 && errno != EEXIST)
    {
        printf("Erro ao criar o diretório %s\n", auth->current_user->name);
        return 1;
    }
    if (get_int("ID do ficheiro: ", &id) != 1)
        return 0;
    if (procurar_por_id(auth->current_user->files, id))
    {
        printf("Já existe um ficheiro com o ID %d.\n", id);
        return 1;
    }
    if (get_line("Nome do ficheiro: ", name, sizeof(name)) != 1)
        return 0;
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s/%s", auth->current_user->name, name);
    auth->current_user->files = adicionar_ficheiro(auth->current_user->files, id, name, fopen(caminho, "a"));
    printf("Ficheiro '%s' adicionado.\n", name);
    return 1;
}

int menu_remove_file(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    int id;

    if (get_int("ID do ficheiro a remover: ", &id) != 1)
        return 0;

    File *curr = procurar_por_id(auth->current_user->files, id);
    if (!curr)
    {
        printf("Ficheiro não encontrado.\n");
        return 1;
    }
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s/%s", auth->current_user->name, curr->name);
    auth->current_user->files = remover_ficheiro(auth->current_user->files, id);
    remove(caminho);
    printf("Ficheiro removido.\n");
    return 1;
}

int menu_list_files(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }
    if (!auth->current_user->files)
    {
        printf("Nenhum ficheiro registado.\n");
        return 1;
    }
    listar_ficheiros(auth->current_user->files);
    return 1;
}

int menu_huffman_compress_file(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    char input[256];
    char output[256];
    int resultado = MKDIR(auth->current_user->name);

    if (resultado != 0 && errno != EEXIST)
    {
        printf("Erro ao criar o diretório %s\n", auth->current_user->name);
        return 1;
    }
    if (get_line("Ficheiro de entrada: ", input, sizeof(input)) != 1)
        return 0;

    if (get_line("Ficheiro comprimido (.huff): ", output, sizeof(output)) != 1)
        return 0;

    FILE *in = fopen(input, "rb");
    if (!in)
    {
        printf("Erro ao abrir ou criar %s\n", input);
        return 1;
    }
    int freq[MAX_CHAR] = {0};
    unsigned char byte;

    while (fread(&byte, 1, 1, in) == 1)
        freq[byte]++;

    rewind(in);

    for (int i = 0; i < MAX_CHAR; i++)
        codigos[i][0] = '\0';

    No *raiz = construirArvore(freq);

    if (!raiz)
    {
        fclose(in);
        printf("Erro ao construir a árvore.\n");
        return 1;
    }
    char codigo[MAX_CODE];
    printf("\nTabela de códigos:\n");
    gerarCodigos(raiz, codigo, 0);
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s/%s.huff", auth->current_user->name, output);
    FILE *out = fopen(caminho, "ab");
    if (!out)
    {
        fclose(in);
        free_arvore(raiz);
        printf("Erro ao criar %s\n", output);
        return 1;
    }
    long tamanhoOriginal = 0;
    for (int i = 0; i < MAX_CHAR; i++)
        tamanhoOriginal += freq[i];

    fwrite(&tamanhoOriginal, sizeof(long), 1, out);
    fwrite(freq, sizeof(int), MAX_CHAR, out);

    unsigned char buffer = 0;
    int bitCount = 0;

    while (fread(&byte, 1, 1, in) == 1)
    {
        char *cod = codigos[byte];
        for (int i = 0; cod[i]; i++)
        {
            buffer <<= 1;
            if (cod[i] == '1')
                buffer |= 1;
            bitCount++;
            if (bitCount == 8)
            {
                fwrite(&buffer, 1, 1, out);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0)
    {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, 1, 1, out);
    }
    fclose(in);
    fclose(out);
    free_arvore(raiz);

    /* Adicionar ficheiro comprimido à lista do utilizador */
    char huff_name[512];
    snprintf(huff_name, sizeof(huff_name), "%s.huff", output);
    int new_id = max_file_id(auth->current_user->files) + 1;
    auth->current_user->files = adicionar_ficheiro(
        auth->current_user->files, new_id, huff_name, fopen(caminho, "a"));

    printf("\nFicheiro comprimido com sucesso.\n");
    printf("Entrada : %s\n", input);
    printf("Saída   : %s (ID %d)\n", huff_name, new_id);
    return 1;
}

int menu_huffman_decompress_file(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    char input[256];
    char output[256];
    int resultado = MKDIR(auth->current_user->name);

    if (resultado != 0 && errno != EEXIST)
    {
        printf("Erro ao criar o diretório %s\n", auth->current_user->name);
        return 1;
    }
    if (get_line("Ficheiro de entrada (caminho completo do ficheiro + .huff)\nEx: (compress/ficheiro.huff): ", input, sizeof(input)) != 1)
        return 0;

    if (get_line("Ficheiro de saída: ", output, sizeof(output)) != 1)
        return 0;
    FILE *in = fopen(input, "rb");
    if (!in)
    {
        printf("Erro ao abrir ou criar %s\n", input);
        return 1;
    }

    long tamanhoOriginal;
    if (fread(&tamanhoOriginal, sizeof(long), 1, in) != 1)
    {
        fclose(in);
        printf("Ficheiro inválido.\n");
        return 1;
    }
    int freq[MAX_CHAR];
    if (fread(freq, sizeof(int), MAX_CHAR, in) != MAX_CHAR)
    {
        fclose(in);
        printf("Cabeçalho inválido.\n");
        return 1;
    }

    No *raiz = construirArvore(freq);

    if (!raiz)
    {
        fclose(in);
        printf("Erro ao reconstruir a árvore.\n");
        return 1;
    }
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s/%s", auth->current_user->name, output);
    FILE *out = fopen(caminho, "ab");
    if (!out)
    {
        fclose(in);
        free_arvore(raiz);
        printf("Erro ao criar %s\n", output);
        return 1;
    }
    if (!raiz->esq && !raiz->dir)
    {
        for (long i = 0; i < tamanhoOriginal; i++)
            fwrite(&raiz->caractere, 1, 1, out);

        fclose(in);
        fclose(out);
        free_arvore(raiz);

        int new_id = max_file_id(auth->current_user->files) + 1;
        auth->current_user->files = adicionar_ficheiro(
            auth->current_user->files, new_id, output, fopen(caminho, "a"));

        printf("\nFicheiro recuperado com sucesso.\n");
        printf("Entrada : %s\n", input);
        printf("Saída   : %s (ID %d)\n", output, new_id);

        return 1;
    }

    unsigned char byte;
    long escritos = 0;

    No *atual = raiz;
    while (escritos < tamanhoOriginal && fread(&byte, 1, 1, in) == 1)
    {
        for (int i = 7; i >= 0; i--)
        {
            int bit = (byte >> i) & 1;

            if (bit == 0)
                atual = atual->esq;
            else
                atual = atual->dir;
            if (!atual->esq && !atual->dir)
            {
                fwrite(&atual->caractere, 1, 1, out);
                escritos++;
                if (escritos == tamanhoOriginal)
                    break;
                atual = raiz;
            }
        }
    }

    fclose(in);
    fclose(out);
    free_arvore(raiz);

    int new_id = max_file_id(auth->current_user->files) + 1;
    auth->current_user->files = adicionar_ficheiro(
        auth->current_user->files, new_id, output, fopen(caminho, "a"));

    printf("\nFicheiro recuperado com sucesso.\n");
    printf("Entrada : %s\n", input);
    printf("Saída   : %s (ID %d)\n", output, new_id);
    printf("Bytes recuperados: %ld\n", tamanhoOriginal);

    return 1;
}

int menu_chat(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    int op;

    do
    {
        clear_screen();
        printf("\n  ================================================\n");
        printf("  MÓDULO DE CHAT\n");
        printf("  ================================================\n");
        printf("  [1] Enviar mensagem\n");
        printf("  [2] Receber próxima mensagem da fila\n");
        printf("  [3] Ver histórico completo\n");
        printf("  [4] Ver histórico por utilizador\n");
        printf("  [5] Estado da fila\n");
        printf("  [0] Voltar ao menu principal\n");
        separator();

        if (get_int("  Opção: ", &op) == EOF)
            return 0;

        switch (op)
        {
        case 1:
            if (!chat_enviar(auth))
                return 0;
            pause_enter();
            break;
        case 2:
            if (!chat_receber(auth))
                return 0;
            pause_enter();
            break;
        case 3:
            if (!chat_historico(auth))
                return 0;
            pause_enter();
            break;
        case 4:
            if (!chat_historico_user(auth))
                return 0;
            pause_enter();
            break;
        case 5:
            chat_status(auth);
            pause_enter();
            break;
        case 0:
            break;
        default:
            printf("  Opção inválida.\n");
            pause_enter();
            break;
        }
    } while (op != 0);

    return 1;
}

int menu_graph(Auth *auth)
{
    if (!auth->current_user)
    {
        printf("Faça login primeiro.\n");
        return 1;
    }

    int op;

    do
    {
        clear_screen();
        printf("\n  ================================================\n");
        printf("  MÓDULO DE PARTILHAS\n");
        printf("  ================================================\n");
        printf("  [1] Registar partilha\n");
        printf("  [2] Ver grafo de partilhas\n");
        printf("  [3] Ver utilizadores no grafo\n");
        printf("  [4] Pesquisa DFS\n");
        printf("  [5] Pesquisa BFS\n");
        printf("  [0] Voltar ao menu principal\n");
        separator();

        if (get_int("  Opção: ", &op) == EOF)
            return 0;

        switch (op)
        {
        case 1:
            if (!graph_registar(auth))
                return 0;
            pause_enter();
            break;
        case 2:
            graph_listar(auth);
            pause_enter();
            break;
        case 3:
            graph_users(auth);
            pause_enter();
            break;
        case 4:
            if (!graph_dfs_menu(auth))
                return 0;
            pause_enter();
            break;
        case 5:
            if (!graph_bfs_menu(auth))
                return 0;
            pause_enter();
            break;
        case 0:
            break;
        default:
            printf("  Opção inválida.\n");
            pause_enter();
            break;
        }
    } while (op != 0);

    return 1;
}

int menu_reports(Auth *auth)
{
    int op;

    do
    {
        clear_screen();
        printf("\n  ================================================\n");
        printf("  MÓDULO DE RELATÓRIOS\n");
        printf("  ================================================\n");
        printf("  [1] Partilhas por utilizador\n");
        printf("  [2] Utilizador que mais partilhou\n");
        printf("  [3] Utilizador que mais recebeu\n");
        printf("  [4] Ficheiros mais partilhados\n");
        printf("  [5] Total de membros\n");
        printf("  [6] Todos os relatórios\n");
        printf("  [0] Voltar ao menu principal\n");
        separator();

        if (get_int("  Opção: ", &op) == EOF)
            return 0;

        if (op >= 1 && op <= 6 && auth->graph->user_count == 0)
        {
            printf("\n  Sem dados. Registe utilizadores e partilhas primeiro.\n");
            pause_enter();
            continue;
        }

        switch (op)
        {
        case 1:
            header("Partilhas por Utilizador");
            report_shares_per_user(auth->graph);
            pause_enter();
            break;
        case 2:
            header("Utilizador que Mais Partilhou");
            report_top_sender(auth->graph);
            pause_enter();
            break;
        case 3:
            header("Utilizador que Mais Recebeu");
            report_top_receiver(auth->graph);
            pause_enter();
            break;
        case 4:
            header("Ficheiros Mais Partilhados");
            report_most_shared_files(auth->graph);
            pause_enter();
            break;
        case 5:
            header("Total de Membros");
            report_total_members(auth->graph);
            pause_enter();
            break;
        case 6:
            report_all(auth->graph);
            pause_enter();
            break;
        case 0:
            break;
        default:
            printf("  Opção inválida.\n");
            pause_enter();
            break;
        }
    } while (op != 0);

    return 1;
}

void print_menu(Auth *auth)
{
    printf("\n=== Sistema de Partilha de Ficheiros ===\n");
    if (auth->current_user)
        printf("Sessão : %s (ID %d)\n",
               auth->current_user->name, auth->current_user->id);
    else
        printf("Sessão : nenhuma\n");
    printf("Chat   : %d mensagem(ns) na fila | "
           "Grafo: %d utilizador(es) | "
           "%d partilha(s)\n",
           auth->message->size,
           auth->graph->user_count,
           auth->graph->total_shares);

    printf("\n-- Membros --\n");
    printf(" 1. Criar membro          2. Remover membro\n");
    printf(" 3. Procurar membro       4. Listar membros\n");
    printf(" 5. Bloquear membro       6. Desbloquear membro\n");

    printf("\n-- Sessão --\n");
    printf(" 7. Login                 8. Logout\n");

    printf("\n-- Ficheiros --\n");
    printf(" 9. Adicionar ficheiro   10. Remover ficheiro\n");
    printf("11. Listar ficheiros\n");

    printf("\n-- Compressão Huffman --\n");
    printf("12. Comprimir texto\n");
    printf("13. Descomprimir texto\n");

    printf("\n-- Chat --\n");
    printf("14. Módulo de Chat\n");

    printf("\n-- Partilhas --\n");
    printf("15. Módulo de Partilhas\n");

    printf("\n-- Relatórios --\n");
    printf("16. Relatórios\n");

    printf("\n17. Limpar ecrã\n");

    printf("\n 0. Sair\n");
    printf("Opção: ");
}