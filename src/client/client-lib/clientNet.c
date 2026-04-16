#include "clientNet.h"

void createTCPClient(NET_Address *adr, int portNumber, AppState state) {
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);

}

void sendTCPData(AppState state, void *data) {
    NET_WriteToStreamSocket(state->tcpClient, data, sizeof(*data));

}