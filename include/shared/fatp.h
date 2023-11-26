#ifndef FATP_H
#define FATP_H

#define BODY_SIZE 512

/**
 * Type of a request
*/
typedef enum {
    UPSERT_DESCRIPTION = 0
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
