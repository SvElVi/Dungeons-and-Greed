#include "clientNet.h"
#define TCP_TIMEOUT 5000
#define NET_DEBUG 1
#define ALLOWED_SERVER_DISTANCE_OUT_OF_SYNC 25 // SQUARED

void clientNetStateLoop(AppState state)
{
    NETPacket packet, inGameTCPPacket;
    static int counter = 0;
    int ingameTCPFlag = 0;

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
        }
        break;

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
            state->serverState = WAITING_FOR_PLAYERS;
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
                state->seed = packet.intData;
                SDL_Log("Server designated %d as seed!\n", state->seed);
                state->gameState = GAME_GENERATE_WORLD;
                break;
            }
        }
        break;

    case GAME_START:
        state->gameState = GAME_PLAYING;
        break;

    case GAME_PLAYING:
        if (readTCPData(state, &inGameTCPPacket, state->tcpClient))
        {
            switch (inGameTCPPacket.command)
            {
            case SERVER_SHUTDOWN:
                state->gameState = GAME_SERVER_SHUTDOWN;
                ingameTCPFlag = 1;
                break;
            }
            if (ingameTCPFlag)
                break;
        }

        checkForDatagram(state, &packet);
        switch (packet.command)
        {
        case UPDATE_CLIENT_PLAYERS:
            updateClientPlayers(state, &packet);
            SDL_Log("CLIENT UDP CHECK: intData=%d PlayerID=%d", packet.intData, packet.PlayerID);
            break;
        }

        counter++;
        if (counter >= 10000)
        {
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
    for (int id = 0; id < MAX_PLAYERS; id++)
    {
        if (playerSyncCheck(state, packet, id))
        {
            continue;
        }
        memcpy(&state->players[id].pos, &packet->players[id].pos, sizeof(Vector2D));         // update position
        memcpy(&state->players[id].flags, &packet->players[id].flags, sizeof(Player_Flags)); // update player flag
        memcpy(&state->players[id].class, &packet->players[id].class, sizeof(Player_Class)); // update player class
        memcpy(&state->players[id].stats, &packet->players[id].stats, sizeof(Stats));        // update stats
        memcpy(&state->players[id].facing, &packet->players[id].facing, sizeof(direction));  // update facing
        memcpy(&state->players[id].flip, &packet->players[id].flip, sizeof(SDL_FlipMode));   // update flip
        // memcpy(&state->players[id].enemyCollisionTimer, &packet->players[id].enemyCollisionTimer, sizeof(Uint32));
        memcpy(&state->players[id].connected, &packet->players[id].connected, sizeof(int)); // update connected
    }
}

bool playerSyncCheck(AppState state, NETPacket *packet, int playerID)
{
    int deltaX = (state->connectedPlayers.players[playerID].pos.x - packet->players[playerID].pos.x);
    int deltaY = (state->connectedPlayers.players[playerID].pos.y - packet->players[playerID].pos.y);

    if ((deltaX * deltaX) + (deltaY * deltaY) > ALLOWED_SERVER_DISTANCE_OUT_OF_SYNC)
    {
        return true;
    }

    return false;
}