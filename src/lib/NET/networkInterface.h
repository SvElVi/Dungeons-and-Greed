#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
#define ADDRESS_LEN 15
#define RESOLVE_ADDRESS_TIMEOUT 5000

#include <SDL3_Net/SDL_net.h>
#include "../inits.h"

typedef enum
{
    REQUESTING_PLAYER_ID,
    APPROVED_PLAYER,
    DENIED_PLAYER,
    CONFIRMING_RECIVED_PLAYER_ID,
    UPDATE_MY_LOCATION,
    UPDATE_PLAYERS_LOCATIONS,
    PLAYER_EXIT,
    PLAYER_EXIT_APPROVED
} NetCommands;

typedef enum
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR
} PlayerID;

typedef struct
{
    NetCommands command;
    PlayerID PlayerID;
    int intData;

} NETPacket;

// This function is executed once during initialization of the game, to initialize SDL_Net.
int startSDLNet(void);

// This function is executed once during the cleanup process, to deinitialize SDL_Net.
void stopSDLNet(void);

// Takes a pointer to the pointer of where to store a datagram socket (UDP),
// and a portnumber then stores that datagram socket in the given location.
void createUDPSocket(NET_DatagramSocket **, int);

// A cleanup function that takes a pointer to a datagram socket and removes
// the datastructure.
void destoryUDPSocket(NET_DatagramSocket *udpSocket);

// Takes AppState to access udpSocket, to check for new UDP packets, and a void** to
// store the new UDP packet if found, which later needs to be typecasted.
void checkForDatagram(AppState state, void **);

// Takes the AppState, a pointer to a RX NET_Address, portnumber and the a pointer to the information
// casted to the void type. Then sends that information as a UDP packet.
void sendDatagram(AppState state, NET_Address *ptrRxAdr, int portnumber, void *data);

bool readTCPData(AppState state, NETPacket *packet, int currentPlayer);

// A blocking function that initializes a network address given in the char* argument,
// and then saves it into the given pointer to pointer space of NET_Address type.
NET_Status initAddress(NET_Address **adress, char *);

#endif