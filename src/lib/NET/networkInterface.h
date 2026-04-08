#include <SDL3_Net/SDL_net.h>
#define ADDRESS_LENGTH 17

int startSDLNet(void);
NET_DatagramSocket* createUDPSocket(int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
NET_Server* createServerSocket(int portNumber);
void destoryServerSocket(NET_Server*);
void initAdress(NET_Address *adress, char ipAdress[ADDRESS_LENGTH]);