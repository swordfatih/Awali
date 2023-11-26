#ifndef CLIENT_H
#define CLIENT_H

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

#define CRLF     "\r\n"
#define PORT     1977

#define BUF_SIZE 1024

typedef enum 
{
    INITIAL,
    FRIEND_LIST,
    GAME_LIST,
    GAME_START,
    PLAY,
    GAME_END,
} State;

typedef struct
{
    State state;
    SOCKET sock;
} Data;

void init(void);
void end(void);
void app(const char *address, const char *name);
int init_connection(const char *address);
void end_connection(int sock);
int read_server(SOCKET sock, char *buffer);
void write_server(SOCKET sock, const char *buffer);

#endif /* guard */
