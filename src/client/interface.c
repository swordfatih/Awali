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
            printf("3. Defier un joueur.\n");
            break;      
        case CHALLENGE:
            printf("1. Accepter.\n");
            printf("2. Refuser.\n");
            break;
        case WAITING:
            printf("En attente du serveur...\n");
            break;
        case MOVE:
            printf("1. Jouer.\n");
            printf("2. Quitter la partie.\n");
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
        case CHALLENGE:
            challenge_choices(data, choice);
            break;
        case MOVE:
            move_choices(data, choice);
            break;
        default: 
            break;
    }

    printf("\n");
}

void initial_choices(Data* data, int choice)
{
    switch(choice) 
    {
        case 1:
            upsert_description_request(data);
            break;
        case 2:
            ask_list_request(data);
            break;
        case 3:
            send_challenge_request(data);
            break;
        default:
            printf("Unhandled initial choice.\n");
            break;
    }
}

void game_choices(Data* data, int choice)
{
    switch(choice) 
    {
        default:
            printf("Unhandled game choice.\n");
            break;
    }
}

void challenge_choices(Data* data, int choice)
{
    switch(choice) 
    {
        case 1:
            answer_challenge_request(data, 1);
            break;
        case 2:
            answer_challenge_request(data, 0);
            break;
        default:
            printf("Unhandled challenge choice.\n");
            break;
    }
}

void move_choices(Data* data, int choice)
{
    switch(choice) 
    {
        case 1:
            send_move_request(data);
            break;
        case 2:
            // quitter la partie
            break;
        default:
            printf("Unhandled move choice.\n");
            break;
    }
}