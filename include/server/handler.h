#ifndef HANDLER_H
#define HANDLER_H

#include "fatp.h"

// Function pointer for request handle functions
typedef Status (*Handler) (Request req);
typedef Handler (*req)();

Handler get_handler(RequestType type);
Request parse_request(char* buffer);

Status upsert_description(Request request);

#endif /* guard */