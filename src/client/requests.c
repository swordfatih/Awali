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
    strcat(request, SEPARATOR);
    strcat(request, type_str);
    strcat(request, SEPARATOR);
    strcat(request, body);
    strcat(request, SEPARATOR);

    return request;
}

void upsert_description_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez votre bio: ");
    scanf("%s", buffer);
    printf("\n");

    char* request = format_request(UPSERT_DESCRIPTION, buffer);
    write_server(data->sock, request);
}

void ask_list_request(Data* data)
{
    char* request = format_request(ASK_LIST, "Vide");
    write_server(data->sock, request);
}

void send_challenge_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez le nom de l'adversaire: ");
    scanf("%s", buffer);
    printf("\n");

    char* request = format_request(SEND_CHALLENGE, buffer);
    write_server(data->sock, request);
    data->state = WAITING;
}

void answer_challenge_request(Data* data, int answer) 
{
    char texte[BUF_SIZE];
    sprintf(texte, "%d", answer);
    char* request = format_request(ANSWER_CHALLENGE, texte);
    write_server(data->sock, request);

    data->state = answer == 0 ? INITIAL : WAITING;
}

void send_move_request(Data* data)
{
    int valid = 0;
    char buff[3];
    char ch;
    do {
        printf("Entrez la case de votre choix : ");
        fgets(buff, 3, stdin);
        if(buff[1] != '\n'){
            while (((ch = getchar()) != '\n') && (ch != EOF)){}
            printf("Trop de caracteres\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);

    char* request = format_request(SEND_MOVE, buff);
    write_server(data->sock, request);
}