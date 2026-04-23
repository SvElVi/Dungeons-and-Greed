#include "clientNet.h"
#define TCP_TIMEOUT 5000
#define NET_DEBUG 1

void createTCPClient(NET_Address *adr, int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);

    switch(NET_WaitUntilConnected(state->tcpClient, TCP_TIMEOUT)) {
        case NET_SUCCESS:
            if (NET_DEBUG) SDL_Log("Created server and connected!\n");
            break;

        case NET_FAILURE:
            if (NET_DEBUG) SDL_Log("Something went wrong!\n");
            break;

        case NET_WAITING:
            if (NET_DEBUG) SDL_Log("Waiting!\n");
            break;

        default:
            SDL_Log("Something went terribly wrong!\n");
    }
}

NET_Status checkStreamsocketConnection(AppState state)
{
    switch (NET_GetConnectionStatus(state->tcpClient))
    {
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

void sendTCPData(AppState state, void *data)
{
    NET_WriteToStreamSocket(state->tcpClient, data, sizeof(NETPacket));
}

void clientTCPHandshake(AppState state)
{
    NETPacket packet = {REQUESTING_PLAYER_ID, 0};

    void *data = &packet;

    sendTCPData(state, data);
}

NET_Status handshakeDone(AppState state)
{
    void *data = NULL;

    switch (NET_ReadFromStreamSocket(state->tcpClient, data, sizeof(NETPacket)))
    {
    case NET_SUCCESS:
        if (data != NULL)
        {
            NETPacket packet = (*(NETPacket *)data);

            if (packet.command == APPROVED_PLAYER)
            {
                SDL_Log("Server: You're playerID is: %d\n", packet.PlayerID);
                return NET_SUCCESS;
            }
        }

    case NET_FAILURE:
        return NET_FAILURE;
    }

    return NET_WAITING;
}