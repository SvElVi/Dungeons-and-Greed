#include "clientNet.h"

#define TCP_TIMEOUT 1000

void createTCPClient(NET_Address *adr, int portNumber, AppState state) {
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);
    
    NET_WaitUntilConnected(state->tcpClient, TCP_TIMEOUT);

}

void sendTCPData(AppState state, void *data) {
    NET_WriteToStreamSocket(state->tcpClient, data, sizeof(*data));

}