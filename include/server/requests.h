#ifndef REQUESTS_H
#define REQUESTS_H

// Return associated function
typedef int (*request_func) (char* buffer);
typedef request_func (*pm)();

request_func request_type(const char* request);
void process_request(char* buffer);

int login(char* body);
int update(char* body);

#endif /* guard */