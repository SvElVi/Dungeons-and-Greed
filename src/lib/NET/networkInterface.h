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

    Player players[MAX_PLAYERS];

} NETPacket;

int startSDLNet(void);

void stopSDLNet(void);

void createUDPSocket(NET_DatagramSocket **, int);

void destoryUDPSocket(NET_DatagramSocket *udpSocket);

void checkForDatagram(AppState state, NETPacket *packet);

void sendDatagram(AppState state, NET_Address *ptrRxAdr, int portnumber, NETPacket *packet);

bool readTCPData(AppState state, NETPacket *packet, NET_StreamSocket *streamSocket);

void sendTCPData(AppState state, NETPacket *packet, NET_StreamSocket *streamSocket);

bool initAddress(NET_Address **adress, char *adr);

#endif