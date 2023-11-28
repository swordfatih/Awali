#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "handler.h"
#include "fatp.h"

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
        default:
            printf("Unhandled request.\n");
            return ERR_BAD_REQUEST;
    }
}

void handle_error(Status status, RequestType request, Data* data)
{
    if(status == ERR_BAD_REQUEST) 
    {
        printf(KRED "Erreur: mauvais input. Veuillez ressayer." KNRM);

        if(request == SEND_CHALLENGE) 
        {
            data->state = INITIAL;
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
    printf("La liste des joueurs:\n%s\n", request.body);
    
    return OK;
}

Status send_challenge_handler(Request request, Data* data)
{
    printf("Vous avez reçu un challenge de: %s\n", request.body);
    data->state = CHALLENGE;

    return OK;
}

Status answer_challenge_handler(Request request, Data* data) 
{
    printf("Votre adversaire a refusé votre demande :(\n");
    data->state = INITIAL;

    return OK;
}

Status send_game_handler(Request request, Data* data) 
{
    int gameOver = strtol(strtok(request.body, "\n"), NULL, 10);
    char* player = strtok(NULL, "\n");
    int score1 = strtol(strtok(NULL, "\n"), NULL, 10);
    int score2 = strtol(strtok(NULL, "\n"), NULL, 10);
    if (strcmp(data->name, player) == 0){
        //C'est a mon tour de jouer + j'ai le score1
        data->state = MOVE;
        if (gameOver){
            if (score1 > score2){
                printf("Vous avez gagné avec %d points contre %d points\nFelicitation\n", score1, score2);
            } else {
                printf("Vous perdez avec %d points contre %d points\n", score1, score2);
            }
            data->state = INITIAL;
        } else {
            printf("Votre score : %d\tScore adversaire : %d\n", score1, score2);
            printf("C'est à vous de jouer\n");
        }
    } else {
        //C'est au tour de l'adv + j'ai le score 2
        data->state = WAITING;
        if (gameOver){
            if (score2 > score1){
                printf("Vous avez gagné avec %d points contre %d points\nFelicitation\n", score2, score1);
            } else {
                printf("Vous perdez avec %d points contre %d points\n", score2, score1);
            }
            data->state = INITIAL;
        } else {
            printf("Votre score : %d\tScore adversaire : %d\n", score2, score1);
            printf("En attente du coup de l'adversaire\n");
        }
    }

    int board[12];
    for(int i = 0; i < 12; ++i)
    {
        board[i] = strtol(strtok(NULL, "\n"), NULL, 10);
    }

    int i;
    for(i = 65; i < 65+6; i++) 
    {
        printf(" %c", i);
    }
    printf("\n");
    for(i = 0; i < 6; i++) 
    {
        printf("|%d", board[i]);
    }
    printf("|\n");
    for(i = 11; i > 5; i--) 
    { 
        printf("|%d", board[i]);
    }
    printf("|\n");
    for(i = 97; i < 97+6; i++) 
    {
        printf(" %c", i);
    }
    printf("\n");

    return OK;
}