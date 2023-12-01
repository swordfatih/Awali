#ifndef CLIENT_H
#define CLIENT_H

#include "fatpp.h"

#define MAX_GAME 1024

typedef enum
{
    FREE,
    BUSY,
    OFFLINE
} ClientState;

typedef struct Client
{
    SOCKET         sock;
    char           name[BUF_SIZE];
    char           description[BUF_SIZE];
    ClientState    status;
    struct Client* current_opponent;
    int            match_idx;
} Client;

const char* client_status_to_string(ClientState s);

#endif /* guard */
