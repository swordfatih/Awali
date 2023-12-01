#include <stdio.h>

#include "interface.h"
#include "requests.h"

void menu(State state)
{
    switch (state)
    {
    case INITIAL:
        printf(BBLU "\n\n[Actions possibles]\n" KCYN);
        printf("1. Mettre à jour votre description.\n");
        printf("2. Voir tous les joueurs.\n");
        printf("3. Defier un joueur.\n");
        printf("4. Observer une partie en cours.\n");
        printf("5. Envoyer un message.\n");
        printf("6. Sortir\n");
        break;
    case CHALLENGE:
        printf(BBLU "\n\n[Actions possibles]\n" KCYN);
        printf("1. Accepter.\n");
        printf("2. Refuser.\n");
        break;
    case WAITING:
        printf(BBLU "En attente du serveur...\n" KCYN);
        break;
    case WAITING_MOVE:
        printf(BBLU "\n\n[Actions possibles]\n" KCYN);
        printf("1. Envoyer un message.\n");
        printf("2. Abandonner la partie.\n");
        break;
    case MOVE:
        printf(BBLU "\n\n[Actions possibles]\n" KCYN);
        printf("1. Jouer un coup.\n");
        printf("2. Envoyer un message.\n");
        printf("3. Abandonner la partie.\n");
        break;
    case SPECTATOR:
        printf(BBLU "\n\n[Actions possibles]\n" KCYN);
        printf("1. Envoyer un message aux spectateurs.\n");
        printf("2. Observer une autre partie en même temps.\n");
        printf("3. Arrêter d'observer toutes les parties.\n");
        break;
    default:
        return;
    }

    printf("\n" KNRM);
    fflush(stdout);
}

void set_state(Data* data, State value)
{
    data->state = value;
    menu(value);
}

int handle_choices(Data* data, int choice)
{
    switch (data->state)
    {
    case INITIAL:
        return initial_choices(data, choice);
    case CHALLENGE:
        return challenge_choices(data, choice);
    case WAITING_MOVE:
        return waiting_move_choices(data, choice);
    case MOVE:
        return move_choices(data, choice);
    case SPECTATOR:
        return spectator_choices(data, choice);
    default:
        break;
    }
    printf("\n");
    return 0;
}

int initial_choices(Data* data, int choice)
{
    switch (choice)
    {
    case 1: // mettre a jour sa bio
        upsert_description_request(data);
        break;
    case 2: // demander la liste des joueurs
        ask_list_request(data);
        break;
    case 3: // envoyer une demande de challende
        send_challenge_request(data);
        break;
    case 4:
        start_spectate_request(data);
        break;
    case 5:
        send_message_request(data);
        break;
    case 6: // quitter le programme
        return -1;
        break;
    default:
        break;
    }
    return 0;
}

int game_choices(Data* data, int choice)
{
    switch (choice)
    {
    default:
        break;
    }
    return 0;
}

int challenge_choices(Data* data, int choice)
{
    switch (choice)
    {
    case 1: // Accepter le challenge
        answer_challenge_request(data, 1);
        break;
    case 2: // refuser le challenge
        answer_challenge_request(data, 0);
        break;
    default:
        break;
    }
    return 0;
}

int waiting_move_choices(Data* data, int choice)
{
    switch (choice)
    {
    case 1: // envoyer un message
        send_chat_request(data);
        break;
    case 2: // quitter la partie
        send_forfait_request(data);
        break;
    default:
        break;
    }
    return 0;
}

int move_choices(Data* data, int choice)
{
    switch (choice)
    {
    case 1: // jouer un coup
        send_move_request(data);
        break;
    case 2: // envoyer un message
        send_chat_request(data);
        break;
    case 3: // quitter la partie
        send_forfait_request(data);
        break;
    default:
        break;
    }
    return 0;
}

int spectator_choices(Data* data, int choice)
{
    switch (choice)
    {
    case 1: // envoyer un message
        send_chat_request(data);
        break;
    case 2: // observer un autre
        start_spectate_request(data);
        break;
    case 3: // arrêter d'observer
        stop_spectate_request(data);
        break;
    default:
        break;
    }

    return 0;
}