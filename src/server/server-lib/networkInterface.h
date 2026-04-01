#include <SDL3_Net/SDL_net.h>

int startSDLNet(void);
NET_DatagramSocket* createUDPSocket(int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
NET_Server* createServerSocket(int portNumber);
void destoryServerSocket(NET_Server*);