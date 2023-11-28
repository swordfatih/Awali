#include "client.h"

const char* client_status_to_string(ClientState s){
   switch (s)
   {
   case FREE:
      return "free";
   case BUSY:
      return "busy";
   case OFFLINE:
      return "offline";
   default:
      return "unknown status";
   }
}