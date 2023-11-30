#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requests.h"
#include "server.h"

void format_request(RequestType type, char* body, char* request)
{
    static int request_id = 0;

    char       id[BUF_SIZE];
    sprintf(id, "%d", request_id);

    char type_str[BUF_SIZE];
    sprintf(type_str, "%d", type);

    strcpy(request, id);
    strcat(request, SEPARATOR);
    strcat(request, type_str);
    strcat(request, SEPARATOR);
    strcat(request, body);
    strcat(request, SEPARATOR);
}

/**
 * Envoie le plateau de jeu à tous les concernés (joueurs et spectateurs)
 */
void broadcast_match(Data* data, int match_idx)
{
    const Match* match = &data->matches.arr[match_idx];

    send_match(match, match->players[0]);
    send_match(match, match->players[1]);

    for (int i = 0; i < match->spectators.nb; ++i)
    {
        if (match->spectators.arr[i] != NULL)
        {
            send_match(match, match->spectators.arr[i]);
        }
    }
}

/**
 * Envoie le plateau de jeu :
 * gameOver;nomJoueur1;scoreJoueur1;nomJoueur2;ScoreJoueur2;n°joueurAct
 * plateau
 */
void send_match(const Match* match, Client* client)
{
    char body[BUF_SIZE] = "\0";
    char buffer[100];

    // game over
    sprintf(buffer, "%d\n", match->gameOver);
    strcat(body, buffer);

    // joueur1 info
    strcat(body, match->players[0]->name);
    sprintf(buffer, "\n%d\n", match->game.score1);
    strcat(body, buffer);

    // joueur2 info
    strcat(body, match->players[1]->name);
    sprintf(buffer, "\n%d\n", match->game.score2);
    strcat(body, buffer);

    // numero du joueur act
    sprintf(buffer, "%d\n", match->current_player);
    strcat(body, buffer);

    // etat de chaque case
    for (int i = 0; i < 12; ++i)
    {
        char value[4];
        sprintf(value, "%d", match->game.board.cases[i]);
        strcat(body, value);
        strcat(body, "\n");
    }

    char request[BUF_SIZE];
    format_request(SEND_GAME, body, request);
    write_client(client->sock, request);
}
