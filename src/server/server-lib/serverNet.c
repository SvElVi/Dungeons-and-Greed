#include "serverNet.h"

void createTCPServer(int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpServer = NET_CreateServer(NULL, portNumber);
}