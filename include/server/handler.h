#ifndef HANDLER_H
#define HANDLER_H

#include "client.h"
#include "fatpp.h"
#include "game.h"

/**
 * @brief Tableau de spectateurs
 * 
 */
typedef struct
{
    int     nb;
    Client* arr[MAX_CLIENTS];
} Spectators;

/**
 * @brief Données d'une partie
 * 
 */
typedef struct
{
    Game       game;
    Client*    players[2];
    int        current_player;
    int        game_over;
    Spectators spectators;
    int public;
} Match;

/**
 * @brief Tableau de parties
 * 
 */
typedef struct
{
    int   nb;
    Match arr[MAX_CLIENTS];
} Matches;

/**
 * @brief Tableau de clients
 * 
 */
typedef struct
{
    int    nb;
    Client arr[MAX_CLIENTS];
} Clients;

/**
 * @brief Données du serveur
 * 
 */
typedef struct
{
    Clients clients;
    Matches matches;
} Data;

/**
 * @brief Déléguer la requête au bon handler
 * 
 * @param request La requête
 * @param data Les données du serveur
 * @param client Le client
 * @return Status de la requête
 */
Status  handle_request(Request request, Data* data, Client* client);

/**
 * @brief Parse une requête
 * 
 * @param buffer La requête non structurée
 * @return Requête structurée
 */
Request parse_request(char* buffer);

/**
 * @brief Les handlers des requêtes
 * 
 * @param request La requête structurée 
 * @param data Les données du serveur
 * @param client Le client source
 * @return Status de la requête
 */
Status  upsert_description(Request request, Data* data, Client* client);
Status  ask_list(Request request, Data* data, Client* client);
Status  send_challenge(Request request, Data* data, Client* client);
Status  answer_challenge(Request request, Data* data, Client* client);
Status  send_move(Request request, Data* data, Client* client);
Status  declare_forfeit(Request request, Data* data, Client* client);
Status  send_chat(Request request, Data* data, Client* client);
Status  start_spectate(Request request, Data* data, Client* client);
Status  stop_spectate(Request request, Data* data, Client* client);
Status  send_message(Request request, Data* data, Client* client);

#endif /* guard */