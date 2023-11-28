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
    printf("Le jeu:\n%s\n", request.body);
    
    char* player = strtok(request.body, "\n");

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

    data->state = strcmp(data->name, player) == 0 ? MOVE : WAITING;

    return OK;
}