#include <stdio.h>

#include "interface.h"
#include "requests.h"

void menu(State state)
{
   printf("[ACTIONS]\n");

   switch(state)
   {
        case INITIAL:
            printf("1. Mettre à jour votre description.\n");
            printf("2. Voir tous les joueurs.\n");
            break;      
        default:
            printf("Aucune action possible.\n");
            break;
   }
}

void handle_choices(Data* data, int choice)
{
    switch(data->state) 
    {
        case INITIAL:
            initial_choices(data, choice);
            break;
        default: 
            printf("Unhandled state.\n");
            break;
    }

    printf("\n");
}

void initial_choices(Data* data, int choice)
{
    switch(choice) 
    {
        case 1:
            upsert_description(data);
            break;
        case 2:
            ask_list(data);
            break;
        default:
            printf("Unhandled choice.\n");
            break;
    }
}

void game_choices(Data* data, int choice)
{
    switch(choice) 
    {
        default:
            printf("Unhandled choice.\n");
            break;
    }
}