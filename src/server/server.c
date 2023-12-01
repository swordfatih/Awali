#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "handler.h"
#include "requests.h"
#include "server.h"

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

void app(void)
{
    srand(time(NULL));

    SOCKET sock = init_connection();
    char   buffer[BUF_SIZE];
    /* the index for the array */
    int max = sock;
    int i = 0;
    /* a data structure with every clients & matches */
    Data data;
    data.matches.nb = 0;
    data.clients.nb = 0;

    fd_set rdfs;

    while (1)
    {
        printf("current number of clients : %d\n", data.clients.nb);
        FD_ZERO(&rdfs);

        /* add STDIN_FILENO */
        FD_SET(fileno(stdin), &rdfs);

        /* add the connection socket */
        FD_SET(sock, &rdfs);

        /* add socket of each client */
        for (i = 0; i < data.clients.nb; i++)
        {
            FD_SET(data.clients.arr[i].sock, &rdfs);
        }

        if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }

        /* something from standard input : i.e keyboard */
        if (FD_ISSET(fileno(stdin), &rdfs))
        {
            /* stop process when type on keyboard */
            break;
        }
        else if (FD_ISSET(sock, &rdfs))
        {
            /* new client */
            SOCKADDR_IN  csin = {0};
            unsigned int sinsize = sizeof csin;
            int          csock = accept(sock, (SOCKADDR*)&csin, &sinsize);
            if (csock == SOCKET_ERROR)
            {
                perror("accept()");
                continue;
            }

            /* after connecting the client sends its name */
            if (read_client(csock, buffer) == -1)
            {
                /* disconnected */
                continue;
            }

            int twice = 0;
            for (i = 0 ; i < data.clients.nb ; i++){
                if (strcmp(buffer, data.clients.arr[i].name) == 0){
                    twice = 1;
                    break;
                }
            }

            if (twice == 0) //C'est OK
            {
                write_client(csock, "1");

                /* what is the new maximum fd ? */
                max = csock > max ? csock : max;

                FD_SET(csock, &rdfs);

                data.clients.arr[data.clients.nb].sock = csock;
                strncpy(data.clients.arr[data.clients.nb].name, buffer, BUF_SIZE - 1);
                data.clients.arr[data.clients.nb].status = FREE;
                data.clients.nb++;

                printf(KGRN "%s joined.\n" KNRM, buffer);
                
            } 
            else // le nom existe deja
            {
                printf("Le joueur est existant\n");
                write_client(csock, "0");
                closesocket(csock);
            }
            
        }
        else
        {
            for (i = 0; i < data.clients.nb; i++)
            {
                /* a client is talking */
                if (FD_ISSET(data.clients.arr[i].sock, &rdfs))
                {
                    int c = read_client(data.clients.arr[i].sock, buffer);
                    /* client disconnected */
                    if (c == 0)
                    {
                        closesocket(data.clients.arr[i].sock);
                        data.clients.arr[i].status = OFFLINE;
                        remove_client(&data, data.clients.arr, i, &(data.clients.nb));
                    }
                    else
                    {
                        Request request = parse_request(buffer);
                        Status  status = handle_request(request, &data, &data.clients.arr[i]);

                        if (status != OK)
                        {
                            printf("Request of type %d and id %d failed\n", request.type, request.id);
                        }

                        char body[BUF_SIZE], type_body[BUF_SIZE];
                        sprintf(body, "%d", status);
                        sprintf(type_body, "%d", request.type);
                        strcat(body, "\n");
                        strcat(body, type_body);
                        strcat(body, "\n");

                        char response[BUF_SIZE];
                        format_request(STATUS, body, response);
                        write_client(data.clients.arr[i].sock, response);
                    }

                    break;
                }
            }
        }
    }

    clear_clients(data.clients);
    end_connection(sock);
}

void clear_clients(Clients clients)
{
    for (int i = 0; i < clients.nb; i++)
    {
        closesocket(clients.arr[i].sock);
    }
}

void remove_client(Data* data, Client* clients, int to_remove, int* actual)
{
    if(clients[to_remove].current_opponent != NULL)
    {
        printf(KRED "%s disconnected.\n" KNRM, clients[to_remove].name);
        Request request = {};
        declare_forfeit(request, data, &clients[to_remove]);
    }

    /* we remove the client in the array */
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
    /* number client - 1 */
    (*actual)--;
}

int init_connection(void)
{
    SOCKET      sock = socket(AF_INET, SOCK_STREAM, 0);
    int         option = 1;
    SOCKADDR_IN sin = {0};

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (bind(sock, (SOCKADDR*)&sin, sizeof sin) == SOCKET_ERROR)
    {
        perror("bind()");
        exit(errno);
    }

    if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
    {
        perror("listen()");
        exit(errno);
    }

    return sock;
}

void end_connection(int sock)
{
    closesocket(sock);
}

int read_client(SOCKET sock, char* buffer)
{
    int n = 0;

    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
        perror("recv()");
        /* if recv error we disonnect the client */
        n = 0;
    }

    buffer[n] = 0;

    return n;
}

void write_client(SOCKET sock, const char* buffer)
{
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

int main(int argc, char** argv)
{
    init();

    app();

    end();

    return EXIT_SUCCESS;
}
