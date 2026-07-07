#ifndef MENU_CHAT_AUX_H
#define MENU_CHAT_AUX_H
#include "auth.h"
#include "avl.h"
#include "user.h"
#include "file.h"
#include "chat.h"
#include "menus.h"
#include "menus_aux.h"
#include "menu_graphs_aux.h"

User *prompt_user(Auth *auth, const char *prompt, int *eof);
int chat_enviar(Auth *auth);
int chat_receber(Auth *auth);
int chat_historico(Auth *auth);
int chat_historico_user(Auth *auth);
void chat_status(Auth *auth);

#endif