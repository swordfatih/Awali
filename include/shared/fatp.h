#ifndef FATP_H
#define FATP_H

#ifdef _WIN32

#include <winsock2.h>

#endif

#ifdef __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif

#define CRLF        "\r\n"
#define PORT         1977
#define MAX_CLIENTS  100
#define BUF_SIZE     1024
#define BODY_SIZE 512

/**
 * Type of a request
*/
typedef enum {
    UPSERT_DESCRIPTION = 0,
    ASK_LIST
} RequestType;

/**
 * Header of a request
*/
typedef struct
{
    int id;
    RequestType type;
    char body[BODY_SIZE];
} Request;

/**
 * Status of a response
*/
typedef enum {
    OK = 0,
    ERR_NOT_FOUND,
    ERR_BAD_REQUEST,
    ERR_TO_LONG,
    ERR_ALREADY_CONNECTED,
    ERR_INGAME,
    ERR_NOT_INGAME
} Status;

#endif /* guard */
