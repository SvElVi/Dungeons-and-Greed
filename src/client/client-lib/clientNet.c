#include "clientNet.h"
#define TCP_TIMEOUT 5000
#define NET_DEBUG 1

void clientNetStateLoop(AppState state)
{
    NETPacket packet;
    static int counter = 0;

    switch (state->gameState)
    {
    case GAME_INIT:
        state->connectedPlayers.amountOfPlayers = 0;
        state->gameState = GAME_MENY;
        break;

    case GAME_NET_INIT:
        createUDPSocket(&state->udpSocket, CLIENT_UDP_PORT);
        state->gameState = GAME_IP_INIT;
        break;

    case GAME_IP_INIT:
        if (initAddress(&state->serverIP, state->hostIP))
            state->gameState = GAME_IP_INIT_CHECK;
        break;

    case GAME_IP_INIT_CHECK:
        switch (NET_GetAddressStatus(state->serverIP))
        {
        case NET_SUCCESS:
            state->gameState = GAME_TCP_INIT;
            break;

        default:
            break;
        }

    case GAME_TCP_INIT:
        createTCPClient(state->serverIP, SERVER_TCP_PORT, state);
        state->gameState = GAME_TCP_HANDSHAKE;
        break;

    case GAME_TCP_HANDSHAKE:
        switch (NET_GetConnectionStatus(state->tcpClient))
        {
        case NET_SUCCESS:
            clientTCPHandshake(state, state->tcpClient);
            state->gameState = GAME_TCP_VERIFYING_HANDSHAKE;
            break;

        case NET_FAILURE:
            break;

        case NET_WAITING:
            break;
        }

        break;

    case GAME_TCP_VERIFYING_HANDSHAKE:
        if (readTCPData(state, &packet, state->tcpClient))
        {

            if (packet.command == APPROVED_PLAYER)
            {
                SDL_Log("Server: You're playerID is: %d\n", packet.PlayerID);
                state->curPlayerPtr = &(state->players[packet.PlayerID]);
                state->curPlayerPtr->playerID = packet.PlayerID;
                packet.command = CONFIRMING_RECIVED_PLAYER_ID;
                sendTCPData(state, &packet, state->tcpClient);
                state->gameState = GAME_WAITING_FOR_OTHER_PLAYERS;
            }
        }
        break;

    case GAME_WAITING_FOR_OTHER_PLAYERS:
        if (readTCPData(state, &packet, state->tcpClient))
        {
            switch (packet.command)
            {
            case UPDATE_WAITING_STATUS:
                state->connectedPlayers.amountOfPlayers = packet.intData;
                break;

            case SERVER_START_GAME:
                state->gameState = GAME_START;
                break;
            }
        }
        break;

    case GAME_START:
        state->gameState = GAME_PLAYING;
        break;

    case GAME_PLAYING:
        checkForDatagram(state, &packet);
        switch(packet.command)
        {
            case UPDATE_CLIENT_PLAYERS:
                updateClientPlayers(state, &packet);
                break;
            default:
                break;
        }

        counter++;
        if (counter >= 10000) {
            counter = 0;
            state->gameState = GAME_UPDATE_MY_LOCATION;

        }
        break;

    case GAME_UPDATE_MY_LOCATION:
        updateMyLocation(state);
        state->gameState = GAME_PLAYING;
        break;
    }
}

void createTCPClient(NET_Address *adr, int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);
}

void clientTCPHandshake(AppState state, NET_StreamSocket *streamSocket)
{
    NETPacket packet = {REQUESTING_PLAYER_ID, 0};

    sendTCPData(state, &packet, streamSocket);
}

void updateMyLocation(AppState state)
{
    const int currentPlayer = state->curPlayerPtr->playerID;
    NETPacket locPacket = {.command = UPDATE_SERVER_PLAYER, .PlayerID = currentPlayer};
    sanitizePlayerStruct(state->curPlayerPtr, &locPacket.players[currentPlayer]);
    sendDatagram(state, state->serverIP, SERVER_UDP_PORT, &locPacket);
}

void updateClientPlayers(AppState state, NETPacket *packet)
{
    for (int i = 0; i < packet->intData; i++)
    {
        memcpy(&state->players[i].pos, &packet->players[i].pos, sizeof(Vector2D));  // update position
        memcpy(&state->players[i].flags, &packet->players[i].flags, sizeof(Player_Flags)); // update player flag
        memcpy(&state->players[i].class, &packet->players[i].class, sizeof(Player_Class)); // update player class
        memcpy(&state->players[i].stats, &packet->players[i].stats, sizeof(Stats)); // update stats
        memcpy(&state->players[i].facing, &packet->players[i].facing, sizeof(direction)); // update facing
        memcpy(&state->players[i].flip, &packet->players[i].flip, sizeof(SDL_FlipMode)); // update flip
        //memcpy(&state->players[i].enemyCollisionTimer, &packet->players[i].enemyCollisionTimer, sizeof(Uint32));
        memcpy(&state->players[i].connected, &packet->players[i].connected, sizeof(int)); // update connected
    }
}