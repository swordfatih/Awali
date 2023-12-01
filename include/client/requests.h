#ifndef REQUESTS_H
#define REQUESTS_H

#include "client.h"
#include "fatpp.h"

/**
 * Format requests with a type and a body
 */
void format_request(RequestType type, char* body, char* request);

/**
 * Update user's description
 */
void upsert_description_request(Data* data);
void ask_list_request(Data* data);
void send_challenge_request(Data* data);
void answer_challenge_request(Data* data, int answer);
void send_move_request(Data* data);
void send_forfait_request(Data* data);
void send_chat_request(Data* data);
void start_spectate_request(Data* data);
void stop_spectate_request(Data* data);

#endif /* guard */