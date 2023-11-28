#ifndef INTERFACE_H
#define INTERFACE_H

#include "client.h"

void menu(State state);
void handle_choices(Data* data, int choice);

void initial_choices(Data* data, int choice);
void challenge_choices(Data* data, int choice);
void game_choices(Data* data, int choice);
void move_choices(Data* data, int choice);

#endif /* guard */