#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "interface.h"

void init(void)
{
#ifdef _WIN32
    WSADATA wsa;
    int     err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

void end(void)
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void app(const char* address, const char* name)
{
    SOCKET sock = init_connection(address);
    char   buffer[BUF_SIZE];
    int    ex = 0;

    fd_set rdfs;

    /* On envoie notre nom */
    write_server(sock, name);

    /* Le serveur repond "0" sir le nom existe deja */
    read_server(sock, buffer);
    if (strcmp(buffer, "0") == 0){
        printf(KRED "[ERREUR] Connexion impossible, ce nom existe deja !\n");
        exit(-1);
    }

    Data data;
    data.sock = sock;
    set_state(&data, INITIAL);
    strcpy(data.name, name);

    while (ex != -1)
    {
        FD_ZERO(&rdfs);

        /* add STDIN_FILENO */
        FD_SET(fileno(stdin), &rdfs);

        /* add the socket */
        FD_SET(sock, &rdfs);

        if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }

        /* something from standard input : i.e keyboard */
        if (FD_ISSET(fileno(stdin), &rdfs))
        {
            fgets(buffer, BUF_SIZE - 1, stdin);

            {
                char* p = NULL;
                p = strstr(buffer, "\n");

                if (p != NULL)
                {
                    *p = 0;
                }
                else
                {
                    /* fclean */
                    buffer[BUF_SIZE - 1] = 0;
                }
            }
            
            /* recupere le choix de l'utilisateur et effectue l'action lié*/
            int choice = strtol(buffer, NULL, 10);
            ex = handle_choices(&data, choice);
        }
        else if (FD_ISSET(sock, &rdfs))
        {
            int n = read_server(sock, buffer);

            /* server down */
            if (n == 0)
            {
                printf("Server disconnected !\n");
                break;
            }

            /* Lecture de la requete envoyer par le serveur */
            Request request = parse_request(buffer);

            /* Le serveur renvoie le status de la requete precedement envoyer */
            if (request.type == STATUS)
            {
                Status      status = strtol(strtok(request.body, "\n"), NULL, 10);
                RequestType type = strtol(strtok(NULL, "\n"), NULL, 10);

                if (status != OK)
                {
                    handle_error(status, type, &data);
                }
            }
            /* On recoie une requete du serveur*/
            else
            {
                handle_request(request, &data);
                printf("\n");
            }
        }
    }

    end_connection(sock);
}

int init_connection(const char* address)
{
    SOCKET          sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN     sin = {0};
    struct hostent* hostinfo;

    if (sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL)
    {
        fprintf(stderr, "Unknown host %s.\n", address);
        exit(EXIT_FAILURE);
    }

    sin.sin_addr = *(IN_ADDR*)hostinfo->h_addr_list[0];
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (connect(sock, (SOCKADDR*)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        perror("connect()");
        exit(errno);
    }

    return sock;
}

void end_connection(int sock)
{
    closesocket(sock);
}

int read_server(SOCKET sock, char* buffer)
{
    int n = 0;

    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
        perror("recv()");
        exit(errno);
    }

    buffer[n] = 0;

    return n;
}

void write_server(SOCKET sock, const char* buffer)
{
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage : %s [address] [pseudo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    init();

    app(argv[1], argv[2]);

    end();

    return EXIT_SUCCESS;
}
