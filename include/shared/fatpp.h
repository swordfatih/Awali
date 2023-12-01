#ifndef FATP_H
#define FATP_H

#ifdef _WIN32

#include <winsock2.h>

#endif

#ifdef __linux__

#include <arpa/inet.h>
#include <netdb.h> /* gethostbyname */
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> /* close */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int                SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr    SOCKADDR;
typedef struct in_addr     IN_ADDR;

#endif

#define CRLF "\r\n"     ///< Fin de ligne
#define PORT 1977       ///< Port du serveur
#define MAX_CLIENTS 100 ///< Nombre maximum de clients
#define MAX_MATCHES 100 ///< Nombre maximum de parties
#define BUF_SIZE 1024   ///< Taille du buffer
#define BODY_SIZE 512   ///< Taille du corps d'une requête
#define SEPARATOR "$"   ///< Séparateur

/**
 * @brief Formattage des couleurs
 *
 */
#define KNRM "\x1B[0m"    ///< reset
#define KRED "\x1B[31m"   ///< red
#define KGRN "\x1B[32m"   ///< green
#define KYEL "\x1B[33m"   ///< yellow
#define KBLU "\x1B[34m"   ///< blue
#define KMAG "\x1B[35m"   ///< magenta
#define KCYN "\x1B[36m"   ///< cyan
#define KWHT "\x1B[37m"   ///< white
#define KGRA "\x1b[90m"   ///< gray
#define BBLU "\033[1;94m" ///< bold blue
#define KBLD "\x1b[1m"    ///< bold
#define BGMAG "\x1b[45m"  ///< background magenta

/**
 * @brief Type d'une requête
 * 
 */
typedef enum
{
    UPSERT_DESCRIPTION = 0,
    ASK_LIST,
    SEND_CHALLENGE,
    ANSWER_CHALLENGE,
    SEND_GAME,
    SEND_MOVE,
    STATUS,
    FORFEIT,
    SEND_CHAT,
    START_SPECTATE,
    STOP_SPECTATE,
    SEND_MESSAGE,
} RequestType;

/**
 * @brief Requête structurée
 * 
 */
typedef struct
{
    int         id;
    RequestType type;
    char        body[BODY_SIZE];
} Request;

/**
 * @brief Les status d'une requête
 * 
 */
typedef enum
{
    OK = 0,
    ERR_NOT_FOUND,
    ERR_BAD_REQUEST,
    ERR_TO_LONG,
    ERR_ALREADY_CONNECTED,
    ERR_BUSY,
    ERR_FREE,
    ERR_PRIVATE,
} Status;

#endif /* guard */
