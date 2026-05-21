#include "networkInterface.h"

#define RESOLVE_ADDRESS_TIMEOUT 5000
#define STREAM_SOCKET_DRAIN_TIMEOUT 1000
#define DEBUG 1

struct networkInterface
{
    // Server IP
    NET_Address *serverIP;

    // UDP
    NET_DatagramSocket *udpSocket;
    NET_Datagram **udpPacket;

    // Server TCP
    NET_Server *tcpServer;
    NET_StreamSocket *tcpClient[MAX_PLAYERS];

    unsigned char tcpBuffer[MAX_PLAYERS][sizeof(NETPacket)];
};

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

NetworkInterface createNetworkInterface()
{
    NetworkInterface ptr = SDL_calloc(1, sizeof(struct networkInterface));
    if (ptr != NULL)
    {
        SDL_Log("Created a networkInterface ADT!\n");
    }
    return ptr;
}

// OK
void destroyNetworkInterface(NetworkInterface networkInterface)
{
    SDL_free(networkInterface);
    if (networkInterface == NULL)
    {
        SDL_Log("Destoryed networkInterface ADT!\n");
    }
}

// OK
void allocUDPPacket(NetworkInterface networkInterface)
{
    netSetDgramContainer(networkInterface, SDL_calloc(1, sizeof(NET_Datagram)));
}

// OK
void createUDPSocket(NetworkInterface networkInterface, int portNumber)
{
    netSetDgramSocket(networkInterface, NET_CreateDatagramSocket(NULL, portNumber));

    if (netGetDgramSocket(networkInterface) != NULL)
    {
        SDL_Log("Listening on all network interfaces on port: %d\n", portNumber);
    }
    else
    {
        SDL_Log("Fatal error: Failed to create UDP socket!\n");
    }
}

// OK
void destoryUDPSocket(NetworkInterface networkInterface)
{
    NET_DestroyDatagramSocket(netGetDgramSocket(networkInterface));
    SDL_Log("Destoryed UDP socket\n");
}

void checkForDatagram(NetworkInterface networkInterface, NETPacket *packet)
{
    if (NET_ReceiveDatagram(netGetDgramSocket(networkInterface), netGetDgramContainer(networkInterface)))
    {
        if ((*netGetDgramContainer(networkInterface)) != NULL)
        {
            memcpy(packet, (*netGetDgramContainer(networkInterface))->buf, sizeof(NETPacket));
            NET_DestroyDatagram(*netGetDgramContainer(networkInterface));
        }
    }
}

void sendDatagram(NetworkInterface networkInterface, NET_Address *ptrRxAdr, int portnumber, NETPacket *packet)
{
    NET_SendDatagram(netGetDgramSocket(networkInterface), ptrRxAdr, portnumber, (void *)packet, sizeof(NETPacket));
}

// Behöver göras om, static var bara en proof of concept, men är RIKTIGT dåligt att använda vidare
bool readTCPData(NETPacket *packet, NET_StreamSocket *streamSocket)
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

    return false;
}

void sendTCPData(NETPacket *packet, NET_StreamSocket *streamSocket)
{
    NET_WriteToStreamSocket(streamSocket, (void *)packet, sizeof(NETPacket));
    NET_WaitUntilStreamSocketDrained(streamSocket, STREAM_SOCKET_DRAIN_TIMEOUT);
}

bool initAddress(NET_Address **adress, char *adr)
{
    int addressCheck = 0;

    while (adr[addressCheck] != '\0')
    {
        if (addressCheck > ADDRESS_LEN)
        {
            SDL_Log("Invalid address, failed to resolve!");
            return false;
        }

        addressCheck++;
    }

    *adress = NET_ResolveHostname(adr);
    return true;
}

NET_Address **netGetServerIP(NetworkInterface networkInterface)
{
    return &networkInterface->serverIP;
}

NET_Address *netGetServerIPForTX(NetworkInterface networkInterface)
{
    return networkInterface->serverIP;
}

void netSetTCPClient(NetworkInterface networkInterface, NET_StreamSocket *streamSocket, int playerID)
{
    networkInterface->tcpClient[playerID] = streamSocket;
}

NET_StreamSocket *netGetStreamSocket(NetworkInterface networkInterface, int playerID)
{
    return networkInterface->tcpClient[playerID];
}

// Pointe-to-pointer
NET_StreamSocket **netGetStreamSocketPtP(NetworkInterface networkInterface, int playerID)
{
    return &networkInterface->tcpClient[playerID];
}

void netSetTCPServer(NetworkInterface networkInterface, NET_Server *server)
{
    networkInterface->tcpServer = server;
}

NET_Server *netGetTCPServer(NetworkInterface networkInterface)
{
    return networkInterface->tcpServer;
}

NET_DatagramSocket *netGetDgramSocket(NetworkInterface networkInterface)
{
    return networkInterface->udpSocket;
}

void netSetDgramSocket(NetworkInterface networkInterface, NET_DatagramSocket *dsocket)
{
    networkInterface->udpSocket = dsocket;
}

NET_Datagram **netGetDgramContainer(NetworkInterface networkInterface)
{
    return networkInterface->udpPacket;
}

void netSetDgramContainer(NetworkInterface networkInterface, void *dgram)
{
    networkInterface->udpPacket = dgram;
}

NETPacket *createNetPacket(NetCommands command, int playerID, uint64_t uint64t) {
    NETPacket* ptr = SDL_calloc(1, sizeof(NETPacket));
    ptr->command = command;
    ptr->PlayerID = playerID;
    ptr->uint64 = uint64t;

    return ptr;

}

void destoryNetPacket(NETPacket *ptr) {
    SDL_free(ptr);
    ptr = NULL;
}