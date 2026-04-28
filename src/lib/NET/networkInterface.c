#include "networkInterface.h"

#define RESOLVE_ADDRESS_TIMEOUT 5000
#define DEBUG 1

int startSDLNet(void)
{
    SDL_Log("Initializing SDL_Net...\n");
    if (NET_Init())
    {
        SDL_Log("SDLNet was initialized!\n");
        return NET_SUCCESS;
    }
    else
    {
        SDL_Log("Fatal error: SDL_Net failed to start!\n");
        return NET_FAILURE;
    }
}

void stopSDLNet(void)
{
    NET_Quit();
    SDL_Log("SDLNet was deinitialized!\n");
}

void createUDPSocket(NET_DatagramSocket **dataGramSocket, int portNumber)
{
    *dataGramSocket = NET_CreateDatagramSocket(NULL, portNumber);

    if (*dataGramSocket != NULL)
    {
        SDL_Log("Listening on all network interfaces on port: %d\n", portNumber);
    }
    else
    {
        SDL_Log("Fatal error: Failed to create UDP socket!\n");
    }
}

void destoryUDPSocket(NET_DatagramSocket *udpSocket)
{
    NET_DestroyDatagramSocket(udpSocket);
    SDL_Log("Destoryed UDP socket\n");
}

void checkForDatagram(AppState state, void **data)
{
    if (NET_ReceiveDatagram(state->udpSocket, state->udpPacket))
    {
        if ((*state->udpPacket) != NULL)
        {
            *data = SDL_calloc(1, sizeof((*state->udpPacket)->buf));
            memccpy(*data, (*state->udpPacket)->buf, 1, sizeof((*state->udpPacket)->buf));
            NET_DestroyDatagram(*state->udpPacket);
            (*state->udpPacket) = NULL;
        }
    }
}

void sendDatagram(AppState state, NET_Address *ptrRxAdr, int portnumber, void *data)
{
    NET_SendDatagram(state->udpSocket, ptrRxAdr, portnumber, data, sizeof(data));
}

bool readTCPData(AppState state, NETPacket *packet, NET_StreamSocket *streamSocket)
{
    static int bufLen = 0;
    static unsigned char tcpBuf[sizeof(NETPacket)];
    bufLen += NET_ReadFromStreamSocket(streamSocket, &tcpBuf, sizeof(NETPacket));

    if (bufLen == sizeof(NETPacket))
    {
        memcpy(packet, &tcpBuf, sizeof(NETPacket));
        bufLen = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void sendTCPData(AppState state, NETPacket *packet, NET_StreamSocket *streamSocket)
{
    NET_WriteToStreamSocket(streamSocket, (void *)packet, sizeof(NETPacket));
}

// Blocking
int initAddress(NET_Address **adress, char *adr)
{
    int addressCheck = 0;

    while (adr[addressCheck] != '\0')
    {
        if (addressCheck > ADDRESS_LEN)
        {
            SDL_Log("Invalid address, failed to resolve!");
            return NET_FAILURE;
        }

        addressCheck++;
    }

    *adress = NET_ResolveHostname(adr);

    switch (NET_WaitUntilResolved(*adress, RESOLVE_ADDRESS_TIMEOUT))
    {
    case NET_SUCCESS:
        SDL_Log("Succesfully resolved address of: %s\n", NET_GetAddressString(*adress));
        return NET_SUCCESS;

    case NET_FAILURE:
        SDL_Log("Failed to resolve address!\n");
        return NET_FAILURE;

    default:
        SDL_Log("Ops... something went terribly wrong when trying to resolve a network address!\n");
        return NET_FAILURE;
    }
}