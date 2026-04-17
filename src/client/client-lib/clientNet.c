#include "clientNet.h"

void createTCPClient(NET_Address *adr, int portNumber, AppState state) {
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);

}

NET_Status checkStreamsocketConnection(AppState state) {
    return NET_SUCCESS;
    switch (NET_GetConnectionStatus(state->tcpClient)) {
        case NET_SUCCESS:
            return NET_SUCCESS;

        case NET_WAITING:
            return NET_WAITING;

        case NET_FAILURE:
            SDL_Log("Error: %s\n", SDL_GetError());
            SDL_ClearError();
            return NET_FAILURE;
    }

}

void sendTCPData(AppState state, void *data) {
    NET_WriteToStreamSocket(state->tcpClient, data, sizeof(*data));

}

void clientTCPHandshake(AppState state) {

}

int handshakeDone(AppState state) {
    return 1;
}