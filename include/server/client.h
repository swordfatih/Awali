#ifndef CLIENT_H
#define CLIENT_H

#include "fatpp.h"

#define MAX_GAME 1024 ///< Nombre maximum de parties simultanées

/**
 * @brief Etats du client
 */
typedef enum
{
    FREE,
    BUSY,
    OFFLINE
} ClientState;

/**
 * @brief Données du client
 */
typedef struct Client
{
    SOCKET         sock;
    char           name[BUF_SIZE];
    char           description[BUF_SIZE];
    ClientState    status;
    struct Client* current_opponent;
    int            match_idx;
    int            public_preference;
} Client;

/**
 * @brief Convertit un état en chaîne de caractères
 * @param s Etat
 * @return const char* Chaîne de caractères
 */
const char* client_status_to_string(ClientState s);

#endif /* guard */
