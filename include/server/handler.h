#ifndef HANDLER_H
#define HANDLER_H

#include "client.h"
#include "fatpp.h"
#include "game.h"

typedef struct
{
    int     nb;
    Client* arr[MAX_CLIENTS];
} Spectators;

typedef struct
{
    Game       game;
    Client*    players[2];
    int        current_player;
    int        game_over;
    Spectators spectators;
    int public;
} Match;

typedef struct
{
    int   nb;
    Match arr[MAX_CLIENTS];
} Matches;

typedef struct
{
    int    nb;
    Client arr[MAX_CLIENTS];
} Clients;

typedef struct
{
    Clients clients;
    Matches matches;
} Data;

Status  handle_request(Request request, Data* data, Client* client);
Request parse_request(char* buffer);

Status  upsert_description(Request request, Data* data, Client* client);
Status  ask_list(Request request, Data* data, Client* client);
Status  send_challenge(Request request, Data* data, Client* client);
Status  answer_challenge(Request request, Data* data, Client* client);
Status  send_move(Request request, Data* data, Client* client);
Status  declare_forfeit(Request request, Data* data, Client* client);
Status  send_chat(Request request, Data* data, Client* client);
Status  start_spectate(Request request, Data* data, Client* client);
Status  stop_spectate(Request request, Data* data, Client* client);

#endif /* guard */