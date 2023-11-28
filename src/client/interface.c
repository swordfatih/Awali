#include <stdio.h>

#include "interface.h"
#include "requests.h"

void menu(State state)
{
   switch(state)
   {
        case INITIAL:
            printf(BBLU "\n\n[Actions possibles]\n" KCYN);
            printf("1. Mettre à jour votre description.\n");
            printf("2. Voir tous les joueurs.\n");
            printf("3. Defier un joueur.\n");
            printf("4. Sortir\n");
            break;      
        case CHALLENGE:
            printf(BBLU "\n\n[Actions possibles]\n" KCYN);
            printf("1. Accepter.\n");
            printf("2. Refuser.\n");
            break;
        case WAITING:
            printf(BBLU "En attente du serveur...\n" KCYN);
            break;
        case MOVE:
            printf(BBLU "\n\n[Actions possibles]\n" KCYN);
            printf("1. Jouer.\n");
            printf("2. Quitter la partie.\n");
            break;
        default:
            return;
   }

   printf("\n" KNRM);
   fflush(stdout);
}

int handle_choices(Data* data, int choice)
{
    switch(data->state) 
    {
        case INITIAL:
            return initial_choices(data, choice);
        case CHALLENGE:
            return challenge_choices(data, choice);
        case MOVE:
            return move_choices(data, choice);
        default:
            break;
    }
    printf("\n");
    return 0;
}

int initial_choices(Data* data, int choice)
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
        case 4:
            return -1;
            break;
        default:
            break;
    }
    return 0;
}

int game_choices(Data* data, int choice)
{
    switch(choice) 
    {
        default:
            break;
    }
    return 0;
}

int challenge_choices(Data* data, int choice)
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
            break;
    }
    return 0;
}

int move_choices(Data* data, int choice)
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
            break;
    }
    return 0;
}