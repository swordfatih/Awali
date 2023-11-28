#ifndef CLIENT_H
#define CLIENT_H

#include "fatp.h"

#define MAX_GAME 1024

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   char description[BUF_SIZE];
   int games[MAX_GAME];
} Client;

#endif /* guard */
