#ifndef REQUESTS_H
#define REQUESTS_H

#include "fatpp.h"
#include "handler.h"

/**
 * Format requests with a type and a body
 */
void format_request(RequestType type, char* body, char* request);

void send_match(const Match* match, Client* client);
void broadcast_match(Data* data, int match_idx);

#endif /* guard */