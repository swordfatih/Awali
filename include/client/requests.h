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
void upsert_description_request(Data* data);
void ask_list_request(Data* data);
void send_challenge_request(Data* data);
void answer_challenge_request(Data* data, int answer);
void send_move_request(Data* data);

#endif /* guard */