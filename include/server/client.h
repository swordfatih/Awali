#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"

#define MAX_GAME 1024

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   char bio[BUF_SIZE];
   int games[MAX_GAME];
} Client;

#endif /* guard */
