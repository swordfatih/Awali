#ifndef REQUESTS_H
#define REQUESTS_H

#include "client.h"
#include "fatp.h"

/**
 * Format requests with a type and a body
*/
char* format_request(RequestType type, char* body);

/**
 * Update user's description
*/
void upsert_description(Data* data);
void ask_list(Data* data);

#endif /* guard */