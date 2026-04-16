#include "serverNet.h"

void createTCPServer(NET_Address *adr, int portNumber, AppState state) {
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpServer = NET_CreateServer(adr, portNumber);

}