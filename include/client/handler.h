#ifndef HANDLER_H
#define HANDLER_H

#include "fatp.h"

typedef enum
{
    INITIAL,
    CHALLENGE,
    WAITING,
    WAITING_MOVE,
    MOVE,
    SPECTATOR,
} State;

typedef struct
{
    State  state;
    SOCKET sock;
    char   name[BUF_SIZE];
} Data;

Status  handle_request(Request request, Data* data);
void    handle_error(Status status, RequestType request, Data* data);
Request parse_request(char* buffer);

Status  ask_list_handler(Request request, Data* data);
Status  send_challenge_handler(Request request, Data* data);
Status  answer_challenge_handler(Request request, Data* data);
Status  send_game_handler(Request request, Data* data);
Status  forfeit_handler(Request request, Data* data);
Status  send_chat_handler(Request request, Data* data);

#endif /* guard */