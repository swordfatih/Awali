#ifndef HANDLER_H
#define HANDLER_H

#include "fatpp.h"

/**
 * @brief Etats du client
 */
typedef enum
{
    INITIAL,      ///< Menu principal
    CHALLENGE,    ///< Défier un joueur
    WAITING,      ///< En attente d'une réponse
    WAITING_MOVE, ///< En attente d'un coup
    MOVE,         ///< Jouer un coup
    SPECTATOR,    ///< Spectateur
} State;

/**
 * @brief Données du client
 */
typedef struct
{
    State  state;
    SOCKET sock;
    char   name[BUF_SIZE];
} Data;

/**
 * @brief Gestions des requêtes
 *
 * @param request Requête
 * @param data    Données du client
 * @return Status de la requête
 */
Status handle_request(Request request, Data* data);

/**
 * @brief Gestion des erreurs
 *
 * @param status  Status de la requête
 * @param request Type de requête concernée
 * @param data    Données du client
 */
void handle_error(Status status, RequestType request, Data* data);

/**
 * @brief Parse une requête
 *
 * @param buffer Buffer
 * @return Requête
 */
Request parse_request(char* buffer);

/**
 * @brief Gestion des requêtes
 *
 * @param request Requête
 * @param data    Données du client
 * @return Status de la requête
 */
Status ask_list_handler(Request request, Data* data);
Status send_challenge_handler(Request request, Data* data);
Status answer_challenge_handler(Request request, Data* data);
Status send_game_handler(Request request, Data* data);
Status forfeit_handler(Request request, Data* data);
Status send_chat_handler(Request request, Data* data);

#endif /* guard */