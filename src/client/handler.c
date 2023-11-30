#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "handler.h"
#include "fatp.h"
#include "interface.h"

Status handle_request(Request request, Data* data)
{
    switch(request.type)
    {
        case ASK_LIST:
            return ask_list_handler(request, data);
        case SEND_CHALLENGE:
            return send_challenge_handler(request, data);
        case ANSWER_CHALLENGE:
            return answer_challenge_handler(request, data);
        case SEND_GAME:
            return send_game_handler(request, data);
        case FORFEIT:
            return forfeit_handler(request, data);
        case SEND_CHAT:
            return send_chat_handler(request, data);
        default:
            printf("Unhandled request.\n");
            return ERR_BAD_REQUEST;
    }
}

void handle_error(Status status, RequestType request, Data* data)
{
    if(status == ERR_BAD_REQUEST) 
    {
        if(request == SEND_CHALLENGE) 
        {
            printf(KRED "[ERREUR] Ce joueur n'existe pas.\n\n" KNRM);
            set_state(data, INITIAL);
        }
        else if(request == SEND_MOVE)
        {
            printf(KRED "[ERREUR] Coup invalide.\n\n" KNRM);
        }
        else
        {
            printf(KRED "[ERREUR] Entree invalide.\n\n" KNRM);
        }
    }
}

Request parse_request(char* buffer) 
{
    Request request;

    request.id = strtol(strtok(buffer, SEPARATOR), NULL, 10);
    request.type = strtol(strtok(NULL, SEPARATOR), NULL, 10);

    char* body = strtok(NULL, SEPARATOR);

    if(body != NULL) 
    {
        strcpy(request.body, body);
    }

    return request;
}

Status ask_list_handler(Request request, Data* data)
{
    printf("La liste des joueurs:\n");
    if(strlen(request.body) <= 1)
    {
        printf(KRED "Il n'y a aucun autre joueur.\n" KNRM);
    }
    else
    {
        printf("%s\n", request.body);
    }
    
    return OK;
}

Status send_challenge_handler(Request request, Data* data)
{
    printf("Vous avez reçu un challenge de: %s\n", request.body);
    set_state(data, CHALLENGE);

    return OK;
}

Status answer_challenge_handler(Request request, Data* data) 
{
    printf("Votre adversaire a refusé votre demande :(\n");
    set_state(data, INITIAL);

    return OK;
}

Status send_game_handler(Request request, Data* data) 
{
    int gameOver = strtol(strtok(request.body, "\n"), NULL, 10);

    char* players[2];
    int scores[2];
    players[0] = strtok(NULL, "\n");
    scores[0] = strtol(strtok(NULL, "\n"), NULL, 10);
    players[1] = strtok(NULL, "\n");
    scores[1] = strtol(strtok(NULL, "\n"), NULL, 10);

    int actual_player = strtol(strtok(NULL, "\n"), NULL, 10);

    int board[12];
    for(int i = 0; i < 12; ++i)
    {
        board[i] = strtol(strtok(NULL, "\n"), NULL, 10);
    }

    int me = (strcmp(data->name, players[0]) == 0) ? 0 : 1;
    int adv = (me+1)%2;

    if (gameOver == 1) {
        if(actual_player == me) 
        {
            printf("Vous avez perdu avec un score de %d contre %d.\nPeut être la suivante !\n", scores[adv], scores[me]);
        }
        else 
        {
            printf("Vous avez gagné avec un score de %d contre %d.\nFélicitations\n", scores[me], scores[adv]);
        }
        set_state(data, INITIAL);
    }
    else {
        if(actual_player == me) 
        {
            set_state(data, MOVE);
            printf(KMAG "\nC'est votre tour.\n\n" KNRM);
        } 
        else 
        {
            set_state(data, WAITING_MOVE);
            printf(KMAG "\nC'est le tour de votre adversaire.\n\n" KNRM);
        }

        printf(KGRN "Votre score: " KNRM "%d\n" KGRN "Score de %s: " KNRM "%d\n", scores[me], players[adv], scores[adv]);
    }

    printf("\n");

    int i;
    if (me == 1) {
        printf(KGRA "┌───┬───┬───┬───┬───┬───┐\n" KNRM);
        for(i = 65; i < 65+6; i++) 
        {
            printf(KGRA "│" KNRM);
            printf(" %c ", i);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 0; i < 6; i++) 
        {
            printf(KGRA "│" KNRM);
            printf(" %d ", board[i]);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 11; i > 5; i--) 
        { 
            printf(KGRA "│" KNRM);
            printf(" %d ", board[i]);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 97; i < 97+6; i++) 
        {
            printf(KGRA "│" KNRM);
            printf(" %c ", i);
        }
        printf(KGRA "│\n└───┴───┴───┴───┴───┴───┘\n" KNRM);
    } else {
        printf(KGRA "┌───┬───┬───┬───┬───┬───┐\n" KNRM);
        for(i = 102; i >= 97 ; i--) 
        {
            printf(KGRA "│" KNRM);
            printf(" %c ", i);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 6; i < 12; i++) 
        {
            printf(KGRA "│" KNRM);
            printf(" %d ", board[i]);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 5; i >= 0; i--) 
        { 
            printf(KGRA "│" KNRM);
            printf(" %d ", board[i]);
        }
        printf(KGRA "│\n├───┼───┼───┼───┼───┼───┤\n" KNRM);
        for(i = 70 ; i >= 65; i--) 
        {
            printf(KGRA "│" KNRM);
            printf(" %c ", i);
        }
        printf(KGRA "│\n└───┴───┴───┴───┴───┴───┘\n" KNRM);
    }

    return OK;
}

Status forfeit_handler(Request request, Data* data)
{
    printf("L'adversaire a déclaré forfait, vous gagnez !\n\n");
    set_state(data, INITIAL);

    return OK;
}

Status send_chat_handler(Request request, Data* data)
{
    char* user = strtok(request.body, "\n");
    char* message = strtok(NULL, "\n");

    printf(KGRN "[%s]" KNRM " %s\n" , user, message);

    return OK;
}