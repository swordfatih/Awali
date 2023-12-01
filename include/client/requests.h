#ifndef REQUESTS_H
#define REQUESTS_H

#include "client.h"
#include "fatpp.h"

/**
 * @brief Formatter les requêtes avec un type et un corps
 *
 * @param type    Type de la requête
 * @param body    Corps de la requête
 * @param request Paramètre de sortie
 */
void format_request(RequestType type, char* body, char* request);

/**
 * @brief Les requêtes du client
 * 
 * @param data Données du client
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
void send_message_request(Data* data);

#endif /* guard */