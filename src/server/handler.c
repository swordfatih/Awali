#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "handler.h"
#include "server.h"
#include "fatp.h"

Handler get_handler(RequestType type)
{
    switch(type)
    {
        case UPSERT_DESCRIPTION:
            return &upsert_description;
        default:
            printf("Unhandled request.\n");
            return NULL;
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

Status upsert_description(Request request)
{
    printf("Received: %s\n", request.body);
    return 0;
}