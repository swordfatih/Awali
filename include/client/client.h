#ifndef CLIENT_H
#define CLIENT_H

#include "handler.h"

void init(void);
void end(void);
void app(const char* address, const char* name);
int  init_connection(const char* address);
void end_connection(int sock);
int  read_server(SOCKET sock, char* buffer);
void write_server(SOCKET sock, const char* buffer);

#endif /* guard */
