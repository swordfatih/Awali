#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "requests.h"

char* format_request(RequestType type, char* body)
{
    static int request_id = 0;

    char id[BUF_SIZE];
    sprintf(id, "%d", request_id);

    char type_str[BUF_SIZE];
    sprintf(type_str, "%d", type);

    char* request = (char*) malloc(BUF_SIZE);
    strcpy(request, id);
    strcat(request, "\n");
    strcat(request, type_str);
    strcat(request, "\n");
    strcat(request, body);
    strcat(request, "\n");

    return request;
}

void upsert_description(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez votre bio: ");
    scanf("%s", buffer);
    printf("\n");

    char* request = format_request(UPSERT_DESCRIPTION, buffer);
    write_server(data->sock, request);
}

void ask_list(Data* data){
    char* request = format_request(ASK_LIST, "Vide");
    write_server(data->sock, request);
}