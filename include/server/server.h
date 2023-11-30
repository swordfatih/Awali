#ifndef SERVER_H
#define SERVER_H

#include "fatp.h"
#include "handler.h"

void init(void);
void end(void);
void app(void);
int  init_connection(void);
void end_connection(int sock);
int  read_client(SOCKET sock, char* buffer);
void write_client(SOCKET sock, const char* buffer);
void remove_client(Data* data, Client* clients, int to_remove, int* actual);
void clear_clients(Clients clients);

#endif /* guard */
