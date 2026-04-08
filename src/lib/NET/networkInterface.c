#include "networkInterface.h"

#define RESOLVE_ADDRESS_TIMEOUT 5000

enum {
    FAIL,
    SUCCESS
};

int startSDLNet(void);
NET_DatagramSocket* createUDPSocket(int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
NET_Server* createServerSocket(int portNumber);
void destoryServerSocket(NET_Server*);
void initAdress(NET_Address **adress);

int startSDLNet(void) {
    SDL_Log("Initializing SDL_Net...\n");
    if(NET_Init()) {
        SDL_Log("Succesfully started SDL_Net!\n");
        return SUCCESS;
        
    } else {
        SDL_Log("Fatal error: SDL_Net failed to start!\n");
        return FAIL;

    }
}

NET_DatagramSocket* createUDPSocket(int portNumber) {
    NET_DatagramSocket *pTemp;
    pTemp = NET_CreateDatagramSocket(NULL, portNumber);

    if (pTemp != NULL) {
        SDL_Log("Listening on all network interfaces on port: %d\n", portNumber);

    } else {
        SDL_Log("Fatal error: Failed to create UDP socket!\n");

    }

    return pTemp;

}

void destoryUDPSocket(NET_DatagramSocket* udpSocket) {
    NET_DestroyDatagramSocket(udpSocket);
    SDL_Log("Destoryed UDP socket at %p\n", udpSocket);
}

// Blocking
void initAddress(NET_Address **adress) {
    *adress = NET_ResolveHostname("127.0.0.1");

    switch(NET_WaitUntilResolved(*adress, RESOLVE_ADDRESS_TIMEOUT)) {
        case NET_SUCCESS:
            SDL_Log("Succesfully resolved address of: %s\n", NET_GetAddressString(*adress));
            break;

        case NET_FAILURE:
            SDL_Log("Failed to resolve address!\n");
            break;

        default:
            SDL_Log("Ops... something went terribly wrong when trying to resolve a network address!\n");
            break;

    }

}