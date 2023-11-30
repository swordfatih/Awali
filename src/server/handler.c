#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "client.h"
#include "fatp.h"
#include "game.h"
#include "handler.h"
#include "requests.h"
#include "server.h"

/**
 * request : requete parsée
 * data : les données de l'application (clients & matches)
 * client : l'envoyeur de la requete
 */
Status handle_request(Request request, Data* data, Client* client)
{
    switch (request.type)
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
    case START_SPECTATE:
        return start_spectate(request, data, client);
    case STOP_SPECTATE:
        return stop_spectate(request, data, client);
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

    if (body != NULL && strlen(body) > 0)
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

    for (int i = 0; i < data->clients.nb; ++i)
    {
        Client cli = data->clients.arr[i];
        if (strcmp(cli.name, client->name) != 0)
        {
            strcat(texte, cli.name);
            strcat(texte, "\n");
            strcat(texte, client_status_to_string(cli.status));
            strcat(texte, "\n");

            if (cli.description == NULL || strlen(cli.description) == 0)
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
    for (int i = 0; i < data->clients.nb; i++)
    {
        if (strcmp(data->clients.arr[i].name, request.body) == 0 && strcmp(client->name, request.body) != 0)
        {
            adversaire = &(data->clients.arr[i]);
            break;
        }
    }

    if (adversaire == NULL)
    {
        client->status = FREE;
        return ERR_BAD_REQUEST;
    }

    if (adversaire->status != FREE)
    {
        client->status = FREE;
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

    if (answer == 0)
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

        broadcast_match(data, client->match_idx);
    }

    return OK;
}

Status send_move(Request request, Data* data, Client* client)
{
    Match*  match = &data->matches.arr[client->match_idx];
    Client* adversaire = client->current_opponent;

    if (validMove(match->game, request.body[0], match->current_player + 1) == 0)
    {
        return ERR_BAD_REQUEST;
    }

    playMove(&match->game, request.body[0]);
    saveMove(&(match->game.board), request.body[0]);

    if (isWin(&(match->game), match->current_player + 1) > 0)
    {
        adversaire->status = FREE;
        adversaire->current_opponent = NULL;

        client->status = FREE;
        client->current_opponent = NULL;

        for(int i = 0; i < match->spectators.nb; ++i)
        {
            Client* spectator = match->spectators.arr[i];
            
            if(spectator != NULL)
            {
                spectator->status = FREE;
            }
        }

        match->gameOver = 1;
    }

    match->current_player = (match->current_player + 1) % 2;

    broadcast_match(data, client->match_idx);

    return OK;
}

Status declare_forfeit(Request request, Data* data, Client* client)
{
    Match*  match = &data->matches.arr[client->match_idx];
    Client* adversaire = client->current_opponent;

    // fin de la partie
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
    char body[BUF_SIZE];
    strcpy(body, client->name);
    strcat(body, "\n");
    strcat(body, request.body);
    strcat(body, "\n");

    char req[BUF_SIZE];
    format_request(SEND_CHAT, body, req);

    Client* adversaire = client->current_opponent;

    if (adversaire != NULL)
    {
        write_client(adversaire->sock, req);

        Match* match = &data->matches.arr[client->match_idx];
        for (int i = 0; i < match->spectators.nb; i++)
        {
            write_client(match->spectators.arr[i]->sock, req);
        }
    }

    for (int i = 0; i < data->matches.nb; ++i)
    {
        Match* match = &data->matches.arr[i];

        if (match != NULL)
        {
            for (int i = 0; i < match->spectators.nb; i++)
            {
                Client* spectator = match->spectators.arr[i];
                if (spectator != NULL && strcmp(spectator->name, client->name) == 0)
                {
                    for (int j = 0; j < match->spectators.nb; j++)
                    {
                        if (match->spectators.arr[j] != NULL && strcmp(match->spectators.arr[j]->name, client->name) != 0)
                        {
                            write_client(match->spectators.arr[j]->sock, req);
                        }
                    }
                }
            }
        }
    }

    return OK;
}

Status start_spectate(Request request, Data* data, Client* client)
{
    client->status = BUSY;

    Client* player = NULL;
    for (int i = 0; i < data->clients.nb; i++)
    {
        if (strcmp(data->clients.arr[i].name, request.body) == 0 && strcmp(client->name, request.body) != 0)
        {
            player = &(data->clients.arr[i]);
            break;
        }
    }

    if (player == NULL)
    {
        client->status = FREE;
        return ERR_BAD_REQUEST;
    }

    if (player->status != BUSY || player->current_opponent == NULL)
    {
        client->status = FREE;
        return ERR_FREE;
    }

    Match*      match = &data->matches.arr[player->match_idx];

    Spectators* spectators = &match->spectators;
    spectators->arr[spectators->nb++] = client;

    send_match(match, client);

    return OK;
}

Status stop_spectate(Request request, Data* data, Client* client)
{
    client->status = FREE;

    for (int i = 0; i < data->matches.nb; ++i)
    {
        Match* match = &data->matches.arr[i];

        if (match != NULL)
        {
            for (int i = 0; i < match->spectators.nb; i++)
            {
                Client* spectator = match->spectators.arr[i];
                if (spectator != NULL && strcmp(spectator->name, client->name) == 0)
                {
                    match->spectators.arr[i] = NULL;
                    return OK;
                }
            }
        }
    }

    return OK;
}