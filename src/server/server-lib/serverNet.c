#include "serverNet.h"

#define TCP_SOCKET_DRAIN_TIMEOUT 2000

void createTCPServer(int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpServer = NET_CreateServer(NULL, portNumber);
}

void updateWaitStatusForClients(AppState state) {
    NETPacket waitingPacket = {.command = UPDATE_WAITING_STATUS, .PlayerID = -1, .intData = state->connectedPlayers.amountOfPlayers};
    for (int index = 0; index < state->connectedPlayers.amountOfPlayers; index++) {
        NET_WriteToStreamSocket(state->connectedPlayers.tcpClient[index], (void *)&waitingPacket, sizeof(NETPacket));
        NET_WaitUntilStreamSocketDrained(state->connectedPlayers.tcpClient[index], TCP_SOCKET_DRAIN_TIMEOUT);
    }

}