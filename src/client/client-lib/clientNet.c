#include "clientNet.h"

void createTCPClient(NET_Address *adr, int portNumber, AppState state)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    state->tcpClient = NET_CreateClient(adr, portNumber);
}

NET_Status checkStreamsocketConnection(AppState state)
{
    return NET_SUCCESS;
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
    NET_WriteToStreamSocket(state->tcpClient, data, sizeof(*data));
}

void clientTCPHandshake(AppState state)
{
    SDL_Log("TEST!");
    NETPacket packet = {REQUESTING_PLAYER_ID, 0};

    void *data;
    data = SDL_calloc(1, sizeof(packet));

    *(NETPacket *)data = packet;

    sendTCPData(state, data);
    SDL_free(data);
}

NET_Status handshakeDone(AppState state)
{
    void *data = NULL;

    NET_ReadFromStreamSocket(state->tcpClient, data, sizeof(NETPacket));

    if (data != NULL)
    {
        NETPacket packet = (*(NETPacket *)data);

        if (packet.command == APPROVED_PLAYER)
        {
            SDL_Log("Server: You're playerID is: %d\n", packet.PlayerID);
            return NET_SUCCESS;
        }

        return NET_FAILURE;
    }
}