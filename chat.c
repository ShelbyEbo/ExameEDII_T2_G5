#include "chat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int next_id = 1;

/* ── Utilitário interno: aloca e preenche uma Message ───── */
static Message *message_create(User *sender,
                               User *receiver,
                               const char *content)
{
    Message *m = (Message *)malloc(sizeof(Message));
    if (!m)
        return NULL;

    m->id = next_id++;
    m->sender = sender;
    m->receiver = receiver;
    strncpy(m->content, content, MAX_MESSAGE_LEN - 1);
    m->content[MAX_MESSAGE_LEN - 1] = '\0';
    m->timestamp = time(NULL);
    m->next = NULL;
    return m;
}

// FILA DE MENSAGENS (FIFO)
void queue_init(MessageQueue *q)
{
    q->front = q->rear = NULL;
    q->size = 0;
}

int queue_enqueue(MessageQueue *q,
                  User *sender,
                  User *receiver,
                  const char *content)
{
    if (q->size >= MAX_MESSAGES)
    {
        fprintf(stderr, "[Chat] Fila cheia.\n");
        return -1;
    }
    Message *m = message_create(sender, receiver, content);
    if (!m)
        return -1;

    if (q->rear == NULL)
    {
        q->front = q->rear = m;
    }
    else
    {
        q->rear->next = m;
        q->rear = m;
    }
    q->size++;
    return m->id;
}

Message *queue_dequeue(MessageQueue *q)
{
    if (q->front == NULL)
        return NULL;

    Message *m = q->front;
    q->front = m->next;
    if (q->front == NULL)
        q->rear = NULL;
    m->next = NULL;
    q->size--;
    return m;
}

int queue_is_empty(const MessageQueue *q)
{
    return q->front == NULL;
}

void queue_destroy(MessageQueue *q)
{
    Message *cur = q->front;
    while (cur)
    {
        Message *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    q->front = q->rear = NULL;
    q->size = 0;
}

// ENVIO / RECEPÇÃO
int chat_send(MessageQueue *q, ChatHistory *hist,
              User *sender, User *receiver,
              const char *content)
{
    Message tmp;
    tmp.id = next_id;
    tmp.sender = sender;
    tmp.receiver = receiver;
    strncpy(tmp.content, content, MAX_MESSAGE_LEN - 1);
    tmp.content[MAX_MESSAGE_LEN - 1] = '\0';

    int id = queue_enqueue(q, sender, receiver, content);
    if (id < 0)
        return -1;

    tmp.id = id;
    history_add(hist, &tmp);

    printf("[Chat] Mensagem #%d enviada: %s -> %s\n", id, tmp.sender->name, tmp.receiver->name);
    return id;
}

Message *chat_receive(MessageQueue *q)
{
    Message *m = queue_dequeue(q);
    if (m)
    {
        printf("[Chat] Mensagem #%d recebida por %s\n", m->id, m->receiver->name);
    }
    else
    {
        printf("[Chat] Sem mensagens na fila.\n");
    }
    return m;
}

// HISTÓRICO
void history_init(ChatHistory *h)
{
    h->head = NULL;
    h->count = 0;
}

void history_add(ChatHistory *h, const Message *m)
{
    Message *copy = (Message *)malloc(sizeof(Message));
    if (!copy)
        return;
    *copy = *m;
    copy->next = NULL;

    if (h->head == NULL)
    {
        h->head = copy;
    }
    else
    {
        Message *cur = h->head;
        while (cur->next)
            cur = cur->next;
        cur->next = copy;
    }
    h->count++;
}

void history_print(const ChatHistory *h)
{
    printf("\n+----------------------------------------------+\n");
    printf("|       Histórico de Mensagens (%d)             |\n", h->count);
    printf("+----------------------------------------------+\n");
    Message *cur = h->head;
    while (cur)
    {
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&cur->timestamp));
        printf("[#%d | %s] %s -> %s : %s\n",
               cur->id, buf, cur->sender->name, cur->receiver->name, cur->content);
        cur = cur->next;
    }
    printf("+----------------------------------------------+\n\n");
}

void history_print_user(const ChatHistory *h, User *user)
{
    printf("\n+----------------------------------------------+\n");
    printf("|       Histórico de '%s'                     |\n", user->name);
    printf("+----------------------------------------------+\n");
    Message *cur = h->head;
    int found = 0;
    while (cur)
    {
        if (strcmp(cur->sender->name, user->name) == 0 ||
            strcmp(cur->receiver->name, user->name) == 0)
        {
            printf("  [#%d] %s -> %s : %s\n",
                   cur->id, cur->sender->name, cur->receiver->name, cur->content);
            found++;
        }
        cur = cur->next;
    }
    if (!found)
        printf("  (sem mensagens)\n");
    printf("+----------------------------------------------+\n\n");
}

void history_destroy(ChatHistory *h)
{
    Message *cur = h->head;
    while (cur)
    {
        Message *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    h->head = NULL;
    h->count = 0;
}
