#include "serverNet.h"
#include "player.h"

#define TCP_SOCKET_DRAIN_TIMEOUT 2000

void createTCPServer(int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    netSetTCPServer(state->ptrNetworkInterface, NET_CreateServer(NULL, portNumber));
}

void broadcastTCPToClients(AppState state, NETPacket *packet)
{
    for (int index = 0; index < state->amountOfPlayers; index++)
    {
        NET_WriteToStreamSocket(netGetStreamSocket(state->ptrNetworkInterface, index), (void *)packet, sizeof(NETPacket));
        NET_WaitUntilStreamSocketDrained(netGetStreamSocket(state->ptrNetworkInterface, index), TCP_SOCKET_DRAIN_TIMEOUT);
    }
}

void updateServerPlayer(AppState state, NETPacket *packet)
{
    const int currentPlayer = packet->PlayerID;
    memcpy(&state->players[currentPlayer].pos, &packet->players[currentPlayer].pos, sizeof(Vector2D));         // update position
    memcpy(&state->players[currentPlayer].flags, &packet->players[currentPlayer].flags, sizeof(Player_Flags)); // update player flag
    memcpy(&state->players[currentPlayer].class, &packet->players[currentPlayer].class, sizeof(Player_Class)); // update player class
    memcpy(&state->players[currentPlayer].stats, &packet->players[currentPlayer].stats, sizeof(Stats));        // update stats
    memcpy(&state->players[currentPlayer].facing, &packet->players[currentPlayer].facing, sizeof(direction));  // update facing
    memcpy(&state->players[currentPlayer].flip, &packet->players[currentPlayer].flip, sizeof(SDL_FlipMode));   // update flip
    // memcpy(&state->players[currentPlayer].enemyCollisionTimer, &packet->players[currentPlayer].enemyCollisionTimer, sizeof(Uint32));
    memcpy(&state->players[currentPlayer].connected, &packet->players[currentPlayer].connected, sizeof(int)); // update connected
    // memcpy(&state->players[currentPlayer], &packet->players[currentPlayer], sizeof(Player));
}

void makeBroadcastPacket(AppState state, NETPacket *packet)
{
    for (int i = 0; i < state->amountOfPlayers; i++)
    {
        sanitizePlayerStruct(&state->players[i], &packet->players[i]);
        // memcpy(&packet->players[i], &state->connectedPlayers.players[i], sizeof(Player));
    }
}

void broadcastTCPDeath(AppState state, int playerID)
{
    NETPacket packet = {0};
    packet.command = PLAYER_DEAD;
    packet.PlayerID = playerID;

    broadcastTCPToClients(state, &packet);
}