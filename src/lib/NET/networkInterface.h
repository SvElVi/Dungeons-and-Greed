#include <SDL3_Net/SDL_net.h>
#define ADDRESS_LENGTH 17

int startSDLNet(void);
void createUDPSocket(NET_DatagramSocket**, int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
NET_Server* createServerSocket(int portNumber);
void destoryServerSocket(NET_Server*);
void initAddress(NET_Address **adress);