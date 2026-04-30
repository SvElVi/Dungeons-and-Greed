#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
#define ADDRESS_LEN 15
#define RESOLVE_ADDRESS_TIMEOUT 5000
#define TCP_PORT 2000
#define UDP_PORT 2020

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
    UPDATE_PLAYERS_LOCATIONS,
    PLAYER_EXIT,
    PLAYER_EXIT_APPROVED
} NetCommands;

typedef struct
{
    NetCommands command;
    int PlayerID;
    int intData;

    PlayerLocations playerLocations[MAX_PLAYERS];

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