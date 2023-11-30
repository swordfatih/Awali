#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "requests.h"

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

void upsert_description_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez votre bio: ");
    scanf("%99[^\n]", buffer);
    printf("\n");

    char request[BUF_SIZE];
    format_request(UPSERT_DESCRIPTION, buffer, request);
    write_server(data->sock, request);
}

void ask_list_request(Data* data)
{
    char request[BUF_SIZE];
    format_request(ASK_LIST, "Vide", request);
    write_server(data->sock, request);
}

void send_challenge_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez le nom du joueur à défier: ");
    scanf("%s", buffer);

    char request[BUF_SIZE];
    format_request(SEND_CHALLENGE, buffer, request);
    write_server(data->sock, request);
    set_state(data, WAITING);
}

void answer_challenge_request(Data* data, int answer)
{
    char texte[BUF_SIZE];
    sprintf(texte, "%d", answer);
    char request[BUF_SIZE];
    format_request(ANSWER_CHALLENGE, texte, request);
    write_server(data->sock, request);

    set_state(data, (answer == 0) ? INITIAL : WAITING);
}

void send_move_request(Data* data)
{
    int  valid = 0;
    char buff[3];
    char ch;

    do
    {
        printf("Entrez la case de votre choix : ");
        fgets(buff, 3, stdin);
        if (buff[1] != '\n')
        {
            while (((ch = getchar()) != '\n') && (ch != EOF))
            {
            }
            printf("Trop de caracteres\n");
        }
        else
        {
            valid = 1;
        }
    } while (valid == 0);

    char request[BUF_SIZE];
    format_request(SEND_MOVE, buff, request);
    write_server(data->sock, request);
}

void send_forfait_request(Data* data)
{
    printf("Vous perdez la partie par abandon!\n");
    char request[BUF_SIZE];
    format_request(FORFEIT, "Vide", request);
    write_server(data->sock, request);
    set_state(data, INITIAL);
}

void send_chat_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez votre message: ");
    scanf("%99[^\n]", buffer);

    char request[BUF_SIZE];
    format_request(SEND_CHAT, buffer, request);
    write_server(data->sock, request);
}

void start_spectate_request(Data* data)
{
    char buffer[BUF_SIZE];
    printf("Entrez le nom du joueur à observer: ");
    scanf("%s", buffer);

    char request[BUF_SIZE];
    format_request(START_SPECTATE, buffer, request);
    write_server(data->sock, request);
    set_state(data, WAITING);
}

void stop_spectate_request(Data* data)
{
    char request[BUF_SIZE];
    format_request(STOP_SPECTATE, "Vide", request);
    write_server(data->sock, request);
    set_state(data, INITIAL);
}