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

Status upsert_description(Request request, Data* data, Client* client)
{
    printf("Received: %s\n", request.body);

    return 0;
}

Status ask_list(Request request, Data* data, Client* client)
{
    char texte[BUF_SIZE];
    texte[0] = '\0';
    Client* cli = data->clients;
    while(cli != NULL && cli->name != NULL && strlen(cli->name) > 0) {
        strcat(texte, cli->name);
        strcat(texte, " : ");
        strcat(texte, client_status_to_string(cli->status));
        strcat(texte, "\n");
        cli++;
    }
    char* req = format_request(ASK_LIST, texte);
    write_client(client->sock, req);
    return OK;
}

Status send_challenge(Request request, Data* data, Client* client)
{
    client->status = BUSY;

    Client* adversaire = NULL;
    for(Client* it = data->clients; it != NULL && it->name != NULL && strlen(it->name) != 0; ++it) 
    {
        if(strcmp(it->name, request.body) == 0) 
        {
            adversaire = it;
            break;
        }
    }

    if(adversaire == NULL) 
    {
        perror("adversaire inconnue");
        return ERR_BAD_REQUEST;
    }

    ///verifier que l'adversaire est dispo
    if(adversaire->status != FREE) 
    {
        perror("adversaire occupe");
        return ERR_INGAME;
    }

    client->current_opponent = adversaire;
    adversaire->current_opponent = client;

    adversaire->status = BUSY;

    char* req = format_request(SEND_CHALLENGE, client->name);
    write_client(adversaire->sock, req);

    return OK;
}

Status answer_challenge(Request request, Data* data, Client* client)
{
    int answer = atoi(request.body);

    if(answer == 0) 
    {
        char* req = format_request(ANSWER_CHALLENGE, "vide");
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
        match.current_player = rand() % 2;
        match.game = initGame();

        data->matches.arr[data->matches.nb++] = match;

        send_game(data, client);
        send_game(data, client->current_opponent);
    }

    return OK;
}

Status send_move(Request request, Data* data, Client* client)
{
    Match* match = &data->matches.arr[client->match_idx];

    if(validMove(match->game, request.body[0], match->current_player + 1) == 0)
    {
        perror("Wrong move");
        return ERR_BAD_REQUEST;
    }

    playMove(&match->game, request.body[0]);

    match->current_player = (match->current_player + 1) % 2;

    send_game(data, client);
    send_game(data, client->current_opponent);

    return OK;
}