#ifndef INTERFACE_H
#define INTERFACE_H

#include "client.h"

/**
 * @brief Menu selon l'état du client
 * @param state Etat du client
 */
void menu(State state);

/**
 * @brief Set the state object
 *
 * @param data  Données du client
 * @param value Nouvel état
 */
void set_state(Data* data, State value);

/**
 * @brief Gestion des choix du menu
 *
 * @param data   Données du client
 * @param choice Choix
 * @return int 1 si quitte le programme, 0 sinon
 */
int handle_choices(Data* data, int choice);

/**
 * @brief Gestion des choix pour chaque menu
 * 
 * @param data   Données du client
 * @param choice Choix
 * @return int 1 si quitte le programme, 0 sinon
 */
int initial_choices(Data* data, int choice);
int challenge_choices(Data* data, int choice);
int game_choices(Data* data, int choice);
int waiting_move_choices(Data* data, int choice);
int move_choices(Data* data, int choice);
int spectator_choices(Data* data, int choice);

#endif /* guard */