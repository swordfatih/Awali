#ifndef INTERFACE_H
#define INTERFACE_H

#include "client.h"

void menu(State state);

/*
 * Handle menu choices of the user
*/
int handle_choices(Data* data, int choice);

int initial_choices(Data* data, int choice);
int challenge_choices(Data* data, int choice);
int game_choices(Data* data, int choice);
int move_choices(Data* data, int choice);

#endif /* guard */