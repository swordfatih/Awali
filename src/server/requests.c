#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "requests.h"
#include "server.h"

char* format_request(RequestType type, char* body)
{
    static int request_id = 0;

    char id[BUF_SIZE];
    sprintf(id, "%d", request_id);

    char type_str[BUF_SIZE];
    sprintf(type_str, "%d", type);

    char* request = (char*) malloc(BUF_SIZE);
    strcpy(request, id);
    strcat(request, SEPARATOR);
    strcat(request, type_str);
    strcat(request, SEPARATOR);
    strcat(request, body);
    strcat(request, SEPARATOR);

    return request;
}
/**
 * Envoie le plateau de jeu : 
 * gameOver;nomJoueur1;scoreJoueur1;nomJoueur2;ScoreJoueur2;n°joueurAct
 * plateau
*/
void send_game(Data* data, Client* client) 
{
    const Match match = data->matches.arr[client->match_idx];

    char body[BUF_SIZE] = "\0";
    char buffer[100];

    //game over
    sprintf(buffer, "%d\n", match.gameOver);
    strcat(body, buffer);

    //joueur1 info
    strcat(body, match.players[0]->name);
    sprintf(buffer, "\n%d\n", match.game.score1);
    strcat(body, buffer);

    //joueur2 info
    strcat(body, match.players[1]->name);
    sprintf(buffer, "\n%d\n", match.game.score2);
    strcat(body, buffer);

    //numero du joueur act
    sprintf(buffer, "%d\n", match.current_player);
    strcat(body, buffer);

    //etat de chaque case
    for(int i = 0; i < 12; ++i) 
    {
        char value[4];
        sprintf(value, "%d", match.game.board.cases[i]);
        strcat(body, value);
        strcat(body, "\n");
    }

    char* request = format_request(SEND_GAME, body);
    write_client(client->sock, request);
    free(request);
}
