#ifndef CLIENT_H
#define CLIENT_H

#include "handler.h"

/*
 * @brief Initialisation
 */
void init(void);

/*
 * @brief Fin du programme
 */
void end(void);

/*
 * @brief Envoie une requête au serveur
 * @param address Adresse du serveur
 * @param name    Nom du joueur
 */
void app(const char* address, const char* name);

/*
 * @brief Initialise la connexion avec le serveur
 * @param address Adresse du serveur
 */
int  init_connection(const char* address);

/*
 * @brief Termine la connexion avec le serveur
 * @param sock Socket
 */
void end_connection(int sock);

/*
 * @brief Lit une requête du serveur
 * @param sock   Socket
 * @param buffer Buffer
 * @return Nombre d'octets lus
 */
int  read_server(SOCKET sock, char* buffer);

/*
 * @brief Envoie une requête au serveur
 * @param sock   Socket
 * @param buffer Buffer
 */
void write_server(SOCKET sock, const char* buffer);

#endif /* guard */
