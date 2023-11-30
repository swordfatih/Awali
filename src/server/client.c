#include "client.h"

const char* client_status_to_string(ClientState s)
{
    switch (s)
    {
    case FREE:
        return KGRN "free" KNRM;
    case BUSY:
        return KRED "busy" KNRM;
    case OFFLINE:
        return KGRA "offline" KNRM;
    default:
        return "unknown status";
    }
}