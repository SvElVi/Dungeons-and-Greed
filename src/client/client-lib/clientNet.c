#include "clientNet.h"
#define TCP_TIMEOUT 5000
#define NET_DEBUG 1

void clientNetStateLoop(AppState state)
{
    NETPacket packet;

    switch (state->gameState)
    {
    case GAME_INIT:
        state->connectedPlayers.amountOfPlayers = 0;
        state->gameState = GAME_MENY;
        break;

    case GAME_NET_INIT:
        createUDPSocket(&state->udpSocket, UDP_PORT);
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
        createTCPClient(state->serverIP, TCP_PORT, state);
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