#ifndef CLIENT_H
#define CLIENT_H

#include "fatp.h"

#define MAX_GAME 1024

typedef enum {
   FREE,
   BUSY,
   OFFLINE
} ClientState;

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   char description[BUF_SIZE];
   int games[MAX_GAME];
   ClientState status;
} Client;

const char* client_status_to_string(ClientState s);

#endif /* guard */
