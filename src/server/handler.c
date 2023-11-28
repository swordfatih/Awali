#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "handler.h"
#include "server.h"
#include "fatp.h"
#include "client.h"

Status handle_request(Request request, Data* data, Client* client)
{
    switch(request.type)
    {
        case UPSERT_DESCRIPTION:
            return upsert_description(request, data, client);
        case ASK_LIST:
            return ask_list(request, data, client);
        default:
            printf("Unhandled request.\n");
            return ERR_BAD_REQUEST;
    }
}

Request parse_request(char* buffer) 
{
    Request request;

    request.id = strtol(strtok(buffer, "\n"), NULL, 10);
    request.type = strtol(strtok(NULL, "\n"), NULL, 10);
    strcpy(request.body, strtok(NULL, "\n"));

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
    write_client(client->sock, texte);
    return OK;
}