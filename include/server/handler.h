#ifndef HANDLER_H
#define HANDLER_H

#include "fatp.h"
#include "client.h"

typedef struct 
{
    Client clients[MAX_CLIENTS];
} Data;

Status handle_request(Request request, Data* data, Client* client);
Request parse_request(char* buffer);

Status upsert_description(Request request, Data* data, Client* client);

#endif /* guard */