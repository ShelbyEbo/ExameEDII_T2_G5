#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chat.h"
#include "graph.h"
#include "report.h"
#include "avl.h"
#include "user.h"
#include "auth.h"

#include "presentation2.h"

#define MAX_USERNAME_LEN 99

static void clear_screen(void)
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

static int input_line(const char *prompt, char *buf, int size)
{
  printf("  %s: ", prompt);
  fflush(stdout);
  buf[0] = '\0';
  if (!fgets(buf, size, stdin))
    return 0;
  buf[strcspn(buf, "\r\n")] = '\0';
  return buf[0] != '\0';
}

static int input_int(const char *prompt)
{
  char buf[32];
  if (!input_line(prompt, buf, sizeof(buf)))
    return -1;
  return atoi(buf);
}

static void pause_enter(void)
{
  char dummy[8];
  printf("\n  Prima ENTER para continuar... ");
  fflush(stdout);
  fgets(dummy, sizeof(dummy), stdin);
  if (!strchr(dummy, '\n'))
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
}

static void separator(void)
{
  printf("  ------------------------------------------------\n");
}

static void header(const char *title)
{
  printf("\n  ================================================\n");
  printf("  %s\n", title);
  printf("  ================================================\n\n");
}

// MENU DE CHAT
User *get_user(Auth *auth, const char *prompt)
{
  char name[MAX_USERNAME_LEN];
  User *user = NULL;
  while (1)
  {
    if (!input_line(prompt, name, sizeof(name)))
    {
      return NULL;
    }

    char msg[100];
    sprintf(msg, "ID do %s", prompt);
    int id = input_int(msg);
    user = find_user(auth->users, id);
    int u = graph_find_user(auth->graph, user);
    if (!u)
    {
      printf("[Aviso] Utilizador '%s' não encontrado. Tente novamente.\n", name);
      continue;
    }
    break;
  }

  return user;
}

File *get_file(User *sender, const char *prompt)
{
  char filename[MAX_FILENAME];
  FileList *files = sender->files;
  while (1)
  {
    if (!input_line(prompt, filename, sizeof(filename)))
    {
      return NULL;
    }

    while (files)
    {
      if (strcmp(files->file->name, filename) == 0)
      {
        return files->file;
      }
      files = files->next;
    }

    printf("[Aviso] Ficheiro '%s' não encontrado. Tente novamente.\n", filename);
  }

  return files->file;
}

void menu_chat_enviar(Auth *auth)
{
  header("CHAT > Enviar Mensagem");
  char content[MAX_MESSAGE_LEN];

  User *s = get_user(auth, "Remetente"), *r = NULL;
  if (s)
  {
    r = get_user(auth, "Destinatario");
  }
  else
  {
    printf("  Cancelado.\n");
    return;
  }

  if (s->blocked || r->blocked)
  {
    printf("[Aviso] Um dos utilizadores está bloqueado. Não é possível enviar mensagem.\n");
    return;
  }

  if (!input_line("Mensagem", content, sizeof(content)))
  {
    printf("  Cancelado.\n");
    return;
  }

  int id = chat_send(auth->message, auth->chat, s, r, content);
  printf(id > 0 ? "\n  Mensagem #%d enviada!\n" : "\n  Erro ao enviar.\n", id);
}

void menu_chat_receber(Auth *auth)
{
  header("CHAT > Receber Proxima Mensagem");
  if (queue_is_empty(auth->message))
  {
    printf("  Fila vazia. Nenhuma mensagem para receber.\n");
    return;
  }
  Message *m = chat_receive(auth->message);
  if (m)
  {
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&m->timestamp));
    separator();
    printf("  ID        : #%d\n", m->id);
    printf("  De        : %s\n", m->sender->name);
    printf("  Para      : %s\n", m->receiver->name);
    printf("  Mensagem  : %s\n", m->content);
    printf("  Timestamp : %s\n", buf);
    separator();
    free(m);
  }
}

void menu_chat_historico(Auth *auth)
{
  header("CHAT > Historico Completo");
  if (auth->chat->count == 0)
  {
    printf("  Sem mensagens.\n");
    return;
  }
  history_print(auth->chat);
}

void menu_chat_historico_user(Auth *auth)
{
  header("CHAT > Historico por Utilizador");
  User *user = get_user(auth, "Utilizador");
  if (!user)
  {
    printf("  Cancelado.\n");
    return;
  }
  history_print_user(auth->chat, user);
}

void menu_chat_status(Auth *auth)
{
  header("CHAT > Estado da Fila");
  printf("  Mensagens na fila : %d\n", auth->message->size);
  printf("  Total no historico: %d\n", auth->chat->count);
}

void menu_chat(Auth *auth)
{
  int op;
  do
  {
    clear_screen();
    printf("\n  ================================================\n");
    printf("  MODULO DE CHAT\n");
    printf("  ================================================\n");
    printf("  [1] Enviar mensagem\n");
    printf("  [2] Receber proxima mensagem da fila\n");
    printf("  [3] Ver historico completo\n");
    printf("  [4] Ver historico por utilizador\n");
    printf("  [5] Estado da fila\n");
    printf("  [0] Voltar ao menu principal\n");
    separator();
    op = input_int("Opcao");
    switch (op)
    {
    case 1:
      menu_chat_enviar(auth);
      pause_enter();
      break;
    case 2:
      menu_chat_receber(auth);
      pause_enter();
      break;
    case 3:
      menu_chat_historico(auth);
      pause_enter();
      break;
    case 4:
      menu_chat_historico_user(auth);
      pause_enter();
      break;
    case 5:
      menu_chat_status(auth);
      pause_enter();
      break;
    case 0:
      break;
    default:
      printf("  Opcao invalida.\n");
      pause_enter();
    }
  } while (op != 0);
}

/* ═════════════════════════════════════════════════════════════
 * MODULO PARTILHAS
 * ═════════════════════════════════════════════════════════════ */

void menu_graph_registar(Auth *auth)
{
  header("PARTILHAS > Registar Partilha");

  User *s = get_user(auth, "Remetente"), *r = NULL;
  if (s)
  {
    r = get_user(auth, "Destinatario");
  }
  else
  {
    printf("  Cancelado.\n");
    return;
  }

  File *file = get_file(s, "Nome do ficheiro");
  if (!file)
  {
    printf("  Cancelado.\n");
    return;
  }

  if (graph_add_share(auth->graph, s, r, file) == 0)
    printf("\n  Partilha registada com sucesso!\n");
  else
    printf("\n  Erro ao registar partilha.\n");
}

void menu_graph_add_user(Auth *auth)
{
  header("PARTILHAS > Adicionar Utilizador");
  User *user = get_user(auth, "Utilizador");

  int idx = graph_add_user(auth->graph, user);
  if (idx >= 0)
    printf("\n  Utilizador '%s' registado (indice %d).\n", user->name, idx);
  else
    printf("\n  Erro ao registar utilizador.\n");
}

void menu_graph_listar(Auth *auth)
{
  header("PARTILHAS > Grafo de Partilhas");
  if (auth->graph->user_count == 0)
  {
    printf("  Sem dados.\n");
    return;
  }
  graph_print(auth->graph);
}

void menu_graph_users(Auth *auth)
{
  header("PARTILHAS > Utilizadores Registados");
  if (auth->graph->user_count == 0)
  {
    printf("  Nenhum utilizador.\n");
    return;
  }
  for (int i = 0; i < auth->graph->user_count; i++)
    printf("  [%d] %s\n", i, auth->graph->users[i]->name);
  printf("\n  Total: %d\n", auth->graph->user_count);
}

void menu_graph_dfs(Auth *auth)
{
  header("PARTILHAS > Pesquisa DFS");
  if (auth->graph->user_count == 0)
  {
    printf("  Sem utilizadores.\n");
    return;
  }
  User *start_user = get_user(auth, "Utilizador de partida");
  if (!start_user)
  {
    printf("  Cancelado.\n");
    return;
  }
  graph_dfs(auth->graph, start_user);
}

void menu_graph_bfs(Auth *auth)
{
  header("PARTILHAS > Pesquisa BFS");
  if (auth->graph->user_count == 0)
  {
    printf("  Sem utilizadores.\n");
    return;
  }
  User *start_user = get_user(auth, "Utilizador de partida");
  if (!start_user)
  {
    printf("  Cancelado.\n");
    return;
  }

  graph_bfs(auth->graph, start_user);
}

void menu_graph(Auth *auth)
{
  int op;
  do
  {
    clear_screen();
    printf("\n  ================================================\n");
    printf("  MODULO DE PARTILHAS\n");
    printf("  ================================================\n");
    printf("  [1] Registar partilha\n");
    printf("  [2] Adicionar utilizador\n");
    printf("  [3] Ver grafo de partilhas\n");
    printf("  [4] Ver utilizadores registados\n");
    printf("  [5] Pesquisa DFS\n");
    printf("  [6] Pesquisa BFS\n");
    printf("  [0] Voltar ao menu principal\n");
    separator();
    op = input_int("Opcao");
    switch (op)
    {
    case 1:
      menu_graph_registar(auth);
      pause_enter();
      break;
    case 2:
      menu_graph_add_user(auth);
      pause_enter();
      break;
    case 3:
      menu_graph_listar(auth);
      pause_enter();
      break;
    case 4:
      menu_graph_users(auth);
      pause_enter();
      break;
    case 5:
      menu_graph_dfs(auth);
      pause_enter();
      break;
    case 6:
      menu_graph_bfs(auth);
      pause_enter();
      break;
    case 0:
      break;
    default:
      printf("  Opcao invalida.\n");
      pause_enter();
    }
  } while (op != 0);
}

/* ═════════════════════════════════════════════════════════════
 * MODULO RELATORIOS
 * ═════════════════════════════════════════════════════════════ */

void menu_reports(Auth *auth)
{
  int op;
  do
  {
    clear_screen();
    printf("\n  ================================================\n");
    printf("  MODULO DE RELATORIOS\n");
    printf("  ================================================\n");
    printf("  [1] Partilhas por utilizador\n");
    printf("  [2] Utilizador que mais partilhou\n");
    printf("  [3] Utilizador que mais recebeu\n");
    printf("  [4] Ficheiros mais partilhados\n");
    printf("  [5] Total de membros\n");
    printf("  [6] Todos os relatorios\n");
    printf("  [0] Voltar ao menu principal\n");
    separator();
    op = input_int("Opcao");

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
      printf("  Opcao invalida.\n");
      pause_enter();
    }
  } while (op != 0);
}

void limpar_dados(Auth *auth)
{
  history_destroy(auth->chat);
  queue_destroy(auth->message);
  graph_destroy(auth->graph);
  queue_init(auth->message);
  history_init(auth->chat);
  graph_init(auth->graph);
  printf("\n  Todos os dados foram apagados.\n");
}

// MENU PRINCIPAL
int menu2(Auth *auth)
{
  int op;
  do
  {
    clear_screen();
    printf("\n  ================================================\n");
    printf("  SISTEMA DE CHAT E PARTILHAS\n");
    printf("  ================================================\n");
    printf("  Estado actual:\n");
    printf("    Mensagens na fila : %d\n", auth->message->size);
    printf("    Historico         : %d mensagem(ns)\n", auth->chat->count);
    printf("    Utilizadores      : %d\n", auth->graph->user_count);
    printf("    Total de partilhas: %d\n", auth->graph->total_shares);
    printf("  ------------------------------------------------\n");
    printf("  [1] Modulo de Chat\n");
    printf("  [2] Modulo de Partilhas\n");
    printf("  [3] Relatorios\n");
    printf("  ------------------------------------------------\n");
    printf("  [8] Carregar dados de demonstracao\n");
    printf("  [9] Limpar todos os dados\n");
    printf("  [0] Sair\n");
    separator();
    op = input_int("Opcao");

    switch (op)
    {
    case 1:
      menu_chat(auth);
      break;
    case 2:
      menu_graph(auth);
      break;
    case 3:
      menu_reports(auth);
      break;
    case 8:
      clear_screen();
      header("Limpar Dados");
      limpar_dados(auth);
      printf("\n  (pressione qualquer tecla)\n");
      pause_enter();
      break;
    case 0:
      printf("\n  A sair... Ate breve!\n\n");
      break;
    default:
      printf("  Opcao invalida.\n");
      pause_enter();
    }
  } while (op != 0);

  history_destroy(auth->chat);
  queue_destroy(auth->message);
  graph_destroy(auth->graph);
  return 0;
}