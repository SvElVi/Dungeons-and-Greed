#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
#define ADDRESS_LEN 15

#include <SDL3_Net/SDL_net.h>
#include "../inits.h"

int startSDLNet(void);
void createUDPSocket(NET_DatagramSocket**, int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
void checkForDatagram(AppState state, void*);
void destoryServerSocket(NET_Server*);
int initAddress(NET_Address **adress, char*);

#endif