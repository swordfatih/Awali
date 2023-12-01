#ifndef SERVER_H
#define SERVER_H

#include "fatpp.h"
#include "handler.h"

/**
 * @brief Initialisation
 * 
 */
void init(void);

/**
 * @brief Fin du programme
 * 
 */
void end(void);

/**
 * @brief Lancement du serveur
 * 
 */
void app(void);

/**
 * @brief Initialisation de la connexion
 * 
 * @return int 
 */
int  init_connection(void);

/**
 * @brief Fin de la connexion
 * 
 * @param sock 
 */
void end_connection(int sock);

/**
 * @brief Lecture du client
 * 
 * @param sock Socket du client
 * @param buffer Buffer
 * @return int Etat de la lecture
 */
int  read_client(SOCKET sock, char* buffer);

/**
 * @brief Ecriture du client
 * 
 * @param sock Socket du client
 * @param buffer Texte à écrire
 */
void write_client(SOCKET sock, const char* buffer);

/**
 * @brief Supprime un client
 * 
 * @param data Données du serveur
 * @param clients Liste des clients
 * @param to_remove Index du client à supprimer
 * @param actual Nombre de clients connectés
 */
void remove_client(Data* data, Client* clients, int to_remove, int* actual);

/**
 * @brief Supprime tous les clients
 * 
 * @param clients Liste des clients
 */
void clear_clients(Clients clients);

#endif /* guard */
