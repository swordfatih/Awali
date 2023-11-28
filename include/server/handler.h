#ifndef HANDLER_H
#define HANDLER_H

#include "fatp.h"
#include "client.h"
#include "game.h"

typedef struct
{
    Game game;
    Client* players[2];
    int current_player;
    int gameOver;
} Match;

typedef struct 
{
    int nb;
    Match arr[MAX_CLIENTS];
} Matches;

typedef struct 
{
    int nb;
    Client arr[MAX_CLIENTS];
} Clients;

typedef struct 
{
    Clients clients;
    Matches matches;
} Data;

Status handle_request(Request request, Data* data, Client* client);
Request parse_request(char* buffer);

Status upsert_description(Request request, Data* data, Client* client);
Status ask_list(Request request, Data* data, Client* client);
Status send_challenge(Request request, Data* data, Client* client);
Status answer_challenge(Request request, Data* data, Client* client);
Status send_move(Request request, Data* data, Client* client);

#endif /* guard */