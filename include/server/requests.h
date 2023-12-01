#ifndef REQUESTS_H
#define REQUESTS_H

#include "fatpp.h"
#include "handler.h"

/**
 * @brief Formatter les requêtes avec un type et un corps
 *
 * @param type    Type de la requête
 * @param body    Corps de la requête
 * @param request Paramètre de sortie
 */
void format_request(RequestType type, char* body, char* request);

/**
 * @brief Envoie le plateau de jeu à un client
 *
 * @param match Le match concerné
 * @param client Le client concerné
 */
void send_match(const Match* match, Client* client);

/**
 * @brief Envoie le plateau de jeu à tous les concernés (joueurs et spectateurs)
 *
 * @param data Données du serveur
 * @param match_idx Index du match
 */
void broadcast_match(Data* data, int match_idx);

#endif /* guard */