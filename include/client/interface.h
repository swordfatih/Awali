#ifndef INTERFACE_H
#define INTERFACE_H

#include "client.h"

void menu(State state);
void set_state(Data* data, State value);

/*
 * Handle menu choices of the user
 */
int handle_choices(Data* data, int choice);

int initial_choices(Data* data, int choice);
int challenge_choices(Data* data, int choice);
int game_choices(Data* data, int choice);
int waiting_move_choices(Data* data, int choice);
int move_choices(Data* data, int choice);
int spectator_choices(Data* data, int choice);

#endif /* guard */