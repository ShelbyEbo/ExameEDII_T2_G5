#include "menu_chat_aux.h"

User *prompt_user(Auth *auth, const char *prompt, int *eof)
{
    int id;
    int r = get_int(prompt, &id);
    if (r == EOF)
    {
        *eof = 1;
        return NULL;
    }
    User *u = find_user(auth->users, id);
    if (!u)
        printf("Utilizador com ID %d não encontrado.\n", id);
    return u;
}

int chat_enviar(Auth *auth)
{
    header("CHAT > Enviar Mensagem");

    int eof = 0;
    User *s = prompt_user(auth, "ID do remetente: ", &eof);
    if (eof) return 0;
    if (!s)  return 1;

    User *r = prompt_user(auth, "ID do destinatário: ", &eof);
    if (eof) return 0;
    if (!r)  return 1;

    if (s->blocked || r->blocked)
    {
        printf("Um dos utilizadores está bloqueado.\n");
        return 1;
    }

    char content[MAX_MESSAGE_LEN];
    if (get_line("Mensagem: ", content, sizeof(content)) != 1)
        return 0;

    int id = chat_send(auth->message, auth->chat, s, r, content);
    printf(id > 0 ? "\n  Mensagem #%d enviada!\n" : "\n  Erro ao enviar.\n", id);
    return 1;
}

int chat_receber(Auth *auth)
{
    header("CHAT > Receber Próxima Mensagem");
    if (queue_is_empty(auth->message))
    {
        printf("  Fila vazia. Nenhuma mensagem para receber.\n");
        return 1;
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
    return 1;
}

int chat_historico(Auth *auth)
{
    header("CHAT > Histórico Completo");
    if (auth->chat->count == 0)
    {
        printf("  Sem mensagens.\n");
        return 1;
    }
    history_print(auth->chat);
    return 1;
}

int chat_historico_user(Auth *auth)
{
    header("CHAT > Histórico por Utilizador");
    int eof = 0;
    User *u = prompt_user(auth, "ID do utilizador: ", &eof);
    if (eof) return 0;
    if (!u)  return 1;
    history_print_user(auth->chat, u);
    return 1;
}

void chat_status(Auth *auth)
{
    header("CHAT > Estado da Fila");
    printf("  Mensagens na fila : %d\n", auth->message->size);
    printf("  Total no histórico: %d\n", auth->chat->count);
}
