#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
#define ADDRESS_LEN 15
#define RESOLVE_ADDRESS_TIMEOUT 5000
#define SERVER_TCP_PORT 2000
#define CLIENT_TCP_PORT 2001
#define SERVER_UDP_PORT 2020
#define CLIENT_UDP_PORT 2021

#include <SDL3_Net/SDL_net.h>
#include "../inits.h"

typedef enum
{
    REQUESTING_PLAYER_ID,
    APPROVED_PLAYER,
    DENIED_PLAYER,
    CONFIRMING_RECIVED_PLAYER_ID,
    SERVER_START_GAME,
    UPDATE_MY_LOCATION,
    UPDATE_SERVER_PLAYER,
    UPDATE_CLIENT_PLAYERS,
    PLAYER_EXIT,
    PLAYER_EXIT_APPROVED,
    SERVER_SHUTDOWN
} NetCommands;

typedef struct
{
    NetCommands command;
    int PlayerID;
    int intData;
    uint64_t uint64;

    Player players[MAX_PLAYERS];

} NETPacket;

typedef struct networkInterface *networkInterface;

// Public
int startSDLNet(void);

// Public
void stopSDLNet(void);

// Public
NetworkInterface createNetworkInterface();

// Public
void destroyNetworkInterface(NetworkInterface networkInterface);

// Public
void allocUDPPacket(NetworkInterface networkInterface);

// Private
void createUDPSocket(NetworkInterface networkInterface, int portNumber);

// Private
void destoryUDPSocket(NetworkInterface networkInterface);

// Private
void checkForDatagram(NetworkInterface networkInterface, NETPacket *packet);

// Private
void sendDatagram(NetworkInterface networkInterface, NET_Address *ptrRxAdr, int portnumber, NETPacket *packet);

// Private
bool readTCPData(NETPacket *packet, NET_StreamSocket *streamSocket);

// Private
void sendTCPData(NETPacket *packet, NET_StreamSocket *streamSocket);

// Private
bool initAddress(NET_Address **adress, char *adr);

// Private
NET_Address **netGetServerIP(NetworkInterface networkInterface);

void netSetTCPClient(NetworkInterface networkInterface, NET_StreamSocket *streamSocket);

NET_StreamSocket *netGetStreamSocket(NetworkInterface networkInterface);

void netSetTCPServer(NetworkInterface networkInterface, NET_Server *server);

NET_Server *netGetTCPServer(NetworkInterface networkInterface);

NET_DatagramSocket *netGetDgramSocket(NetworkInterface networkInterface);

void netSetDgramSocket(NetworkInterface networkInterface, NET_DatagramSocket *dsocket);

NET_Datagram **netGetDgramContainer(NetworkInterface networkInterface);

void netSetDgramContainer(NetworkInterface networkInterface, void *);

NET_Address *netGetServerIPForTX(NetworkInterface networkInterface);

#endif