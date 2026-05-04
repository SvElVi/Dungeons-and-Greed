#include "serverNet.h"

#define TCP_SOCKET_DRAIN_TIMEOUT 2000

void createTCPServer(int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpServer = NET_CreateServer(NULL, portNumber);
}

void broadcastToClients(AppState state, NetCommands command, int playerID, int intData)
{
    NETPacket packetToSend = {.command = command, .PlayerID = playerID, .intData = intData};
    for (int index = 0; index < state->connectedPlayers.amountOfPlayers; index++)
    {
        NET_WriteToStreamSocket(state->connectedPlayers.tcpClient[index], (void *)&packetToSend, sizeof(NETPacket));
        NET_WaitUntilStreamSocketDrained(state->connectedPlayers.tcpClient[index], TCP_SOCKET_DRAIN_TIMEOUT);
    }
}

void updateServerPlayer(AppState state, NETPacket *packet) {
    const int currentPlayer = packet->PlayerID;
    memcpy(&state->connectedPlayers.players[currentPlayer].pos, &packet->players[currentPlayer].pos, sizeof(Vector2D));  // update position
    memcpy(&state->connectedPlayers.players[currentPlayer].flags, &packet->players[currentPlayer].flags, sizeof(Player_Flags)); // update player flag
    memcpy(&state->connectedPlayers.players[currentPlayer].class, &packet->players[currentPlayer].class, sizeof(Player_Class)); // update player class
    memcpy(&state->connectedPlayers.players[currentPlayer].stats, &packet->players[currentPlayer].stats, sizeof(Stats)); // update stats
    memcpy(&state->connectedPlayers.players[currentPlayer].facing, &packet->players[currentPlayer].facing, sizeof(direction)); // update facing
    memcpy(&state->connectedPlayers.players[currentPlayer].flip, &packet->players[currentPlayer].flip, sizeof(SDL_FlipMode)); // update flip
    //memcpy(&state->connectedPlayers.players[currentPlayer].enemyCollisionTimer, &packet->players[currentPlayer].enemyCollisionTimer, sizeof(Uint32));
    memcpy(&state->connectedPlayers.players[currentPlayer].connected, &packet->players[currentPlayer].connected, sizeof(int)); // update connected
    
    //memcpy(&state->connectedPlayers.players[currentPlayer], &packet->players[currentPlayer], sizeof(Player));

}