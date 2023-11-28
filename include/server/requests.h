#ifndef REQUESTS_H
#define REQUESTS_H

#include "fatp.h"
#include "handler.h"

/**
 * Format requests with a type and a body
*/
char* format_request(RequestType type, char* body);

void send_game(Data* data, Client* client);

#endif /* guard */