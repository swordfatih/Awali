#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "handler.h"
#include "server.h"
#include "fatp.h"
#include "client.h"
#include "requests.h"
#include "game.h"

/**
 * request : requete parsée
 * data : les données de l'application (clients & matches)
 * client : l'envoyeur de la requete
*/
Status handle_request(Request request, Data* data, Client* client)
{
    switch(request.type)
    {
        case UPSERT_DESCRIPTION:
            return upsert_description(request, data, client);
        case ASK_LIST:
            return ask_list(request, data, client);
        case SEND_CHALLENGE:
            return send_challenge(request, data, client);
        case ANSWER_CHALLENGE:
            return answer_challenge(request, data, client);
        case SEND_MOVE:
            return send_move(request, data, client);
        case FORFEIT:
            return declare_forfeit(request, data, client);
        case SEND_CHAT:
            return send_chat(request, data, client);
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

    if(body != NULL && strlen(body) > 0) 
    {
        strcpy(request.body, body);
    }

    return request;
}

Status upsert_description(Request request, Data* data, Client* client)
{
    strcpy(client->description, request.body);

    return OK;
}

Status ask_list(Request request, Data* data, Client* client)
{
    char texte[BUF_SIZE];
    texte[0] = '\0';

    char count[BUF_SIZE];
    sprintf(count, "%d", data->clients.nb - 1);
    strcat(texte, count);
    strcat(texte, "\n");

    for (int i = 0 ; i < data->clients.nb; ++i) {
        Client cli = data->clients.arr[i];
        if (strcmp(cli.name, client->name) != 0)
        {
            strcat(texte, cli.name);
            strcat(texte, "\n");
            strcat(texte, client_status_to_string(cli.status));
            strcat(texte, "\n");

            if(cli.description == NULL || strlen(cli.description) == 0)
            {
                strcat(texte, "0\n");
            }
            else
            {
            printf("desc: %s\n", cli.description);
                strcat(texte, "1\n");
                strcat(texte, cli.description);
                strcat(texte, "\n");
            }
        }
    }

    char req[BUF_SIZE];
    format_request(ASK_LIST, texte, req);
    write_client(client->sock, req);

    return OK;
}

Status send_challenge(Request request, Data* data, Client* client)
{
    client->status = BUSY;

    Client* adversaire = NULL;
    for (int i = 0 ; i < data->clients.nb ; i++)
    {
        if(strcmp(data->clients.arr[i].name, request.body) == 0 && strcmp(client->name, request.body) != 0) 
        {
            adversaire = &(data->clients.arr[i]);
            break;
        }
    }

    if(adversaire == NULL) 
    {
        return ERR_BAD_REQUEST;
    }

    if(adversaire->status != FREE) 
    {
        return ERR_BUSY;
    }

    client->current_opponent = adversaire;
    adversaire->current_opponent = client;

    adversaire->status = BUSY;

    char req[BUF_SIZE];
    format_request(SEND_CHALLENGE, client->name, req);
    write_client(adversaire->sock, req);

    return OK;
}

Status answer_challenge(Request request, Data* data, Client* client)
{
    int answer = atoi(request.body);

    if(answer == 0) 
    {
        char req[BUF_SIZE];
        format_request(ANSWER_CHALLENGE, "vide", req);
        write_client(client->current_opponent->sock, req);
        
        client->current_opponent->status = FREE;
        client->current_opponent->current_opponent = NULL;

        client->status = FREE;
        client->current_opponent = NULL;
    } 
    else
    {
        Match match;
        match.players[0] = client;
        match.players[1] = client->current_opponent;
        match.gameOver = 0;
        match.current_player = rand() % 2;
        match.game = initGame();

        client->match_idx = data->matches.nb;
        client->current_opponent->match_idx = data->matches.nb;

        data->matches.arr[data->matches.nb] = match;
        data->matches.nb++;

        send_game(data, client);
        send_game(data, client->current_opponent);
    }

    return OK;
}

Status send_move(Request request, Data* data, Client* client)
{
    Match* match = &data->matches.arr[client->match_idx];
    Client *adversaire = client->current_opponent;

    if(validMove(match->game, request.body[0], match->current_player + 1) == 0)
    {
        return ERR_BAD_REQUEST;
    }

    playMove(&match->game, request.body[0]);
    saveMove(&(match->game.board), request.body[0]);

    if (isWin(&(match->game), match->current_player + 1) > 0){
        adversaire->status = FREE;
        adversaire->current_opponent = NULL;

        client->status = FREE;
        client->current_opponent = NULL;

        match->gameOver = 1;
    }

    match->current_player = (match->current_player + 1) % 2;

    send_game(data, client);
    send_game(data, adversaire);

    return OK;
}

Status declare_forfeit(Request request, Data* data, Client* client){
    Match* match = &data->matches.arr[client->match_idx];
    Client* adversaire = client->current_opponent;

    //fin de la partie
    adversaire->status = FREE;
    adversaire->current_opponent = NULL;

    client->status = FREE;
    client->current_opponent = NULL;

    match->gameOver = 1;

    char req[BUF_SIZE];
    format_request(FORFEIT, "Vide", req);
    write_client(adversaire->sock, req);

    return OK;
}

Status send_chat(Request request, Data* data, Client* client)
{
    printf("message: %s\n", request.body);
    Client* adversaire = client->current_opponent;

    char body[BUF_SIZE];
    strcpy(body, client->name);
    strcat(body, "\n");
    strcat(body, request.body);
    strcat(body, "\n");

    char req[BUF_SIZE];
    format_request(SEND_CHAT, body, req);
    write_client(adversaire->sock, req);

    return OK;
}