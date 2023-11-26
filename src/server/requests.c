#include <string.h>
#include <stdint.h>

#include "requests.h"
#include "server.h"

request_func request_handler(const char* type)
{
    if (strcmp(type, "login") == 0) 
    {
        return &login;
    }
    else if (strcmp(type, "update") == 0)
    {
        return &update;
    }
    else
    {
        return NULL;
    }
}

void process_request(char* buffer) 
{
    char* id = strtok(buffer, "\n");
    char* type = strtok(buffer, "\n");
    char* body = strtok(buffer, "\n");

    int error = request_handler(type)(body);

    printf(error == 0 ? "Request %s from %s processed successfully\n" : "Request %s from %s failed\n", type, id);
}

int login(char* body)
{

}

int update(char* body)
{

}