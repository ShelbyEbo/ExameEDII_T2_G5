#ifndef CHAT_H
#define CHAT_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

#define MAX_MESSAGE_LEN 512
#define MAX_MESSAGES    1000

typedef struct Message
{
    int             id;
    User           *sender;
    User           *receiver;
    char            content[MAX_MESSAGE_LEN];
    time_t          timestamp;
    struct Message *next;
} Message;

typedef struct MessageQueue
{
    Message *front;
    Message *rear;
    int      size;
} MessageQueue;

typedef struct ChatHistory
{
    Message *head;
    int      count;
} ChatHistory;

void     queue_init(MessageQueue *q);
int      queue_enqueue(MessageQueue *q, User *sender,
                       User *receiver, const char *content);
Message *queue_dequeue(MessageQueue *q);
int      queue_is_empty(const MessageQueue *q);
void     queue_destroy(MessageQueue *q);

int      chat_send(MessageQueue *q, ChatHistory *hist,
                   User *sender, User *receiver,
                   const char *content);
Message *chat_receive(MessageQueue *q);

void     history_init(ChatHistory *h);
void     history_add(ChatHistory *h, const Message *m);
void     history_print(const ChatHistory *h);
void     history_print_user(const ChatHistory *h, User *user);
void     history_destroy(ChatHistory *h);

#endif
