#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <SDL3_Net/SDL_net.h>
#define ADDRESS_LENGTH 17

int startSDLNet(void);
void createUDPSocket(NET_DatagramSocket**, int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
void destoryServerSocket(NET_Server*);
void initAddress(NET_Address **adress);

#endif