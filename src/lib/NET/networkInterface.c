#include "networkInterface.h"

#define RESOLVE_ADDRESS_TIMEOUT 5000

enum {
    FAIL,
    SUCCESS
};

int startSDLNet(void);
void createUDPSocket(NET_DatagramSocket**, int);
void destoryUDPSocket(NET_DatagramSocket* udpSocket);
void destoryServerSocket(NET_Server*);
int initAddress(NET_Address **adress, char*);

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

void createUDPSocket(NET_DatagramSocket **dataGramSocket ,int portNumber) {
    *dataGramSocket = NET_CreateDatagramSocket(NULL, portNumber);

    if (*dataGramSocket != NULL) {
        SDL_Log("Listening on all network interfaces on port: %d\n", portNumber);

    } else {
        SDL_Log("Fatal error: Failed to create UDP socket!\n");

    }

}

void destoryUDPSocket(NET_DatagramSocket* udpSocket) {
    NET_DestroyDatagramSocket(udpSocket);
    SDL_Log("Destoryed UDP socket at %p\n", udpSocket);
}

// Blocking
int initAddress(NET_Address **adress, char *adr) {
    int addressCheck = 0;

    while (adr[addressCheck] != '\0') {
        if (addressCheck > ADDRESS_LEN) {
            SDL_Log("Invalid address, failed to resolve!");
            return NET_FAILURE;

        }
        
        addressCheck++;
    }

    *adress = NET_ResolveHostname(adr);

    switch(NET_WaitUntilResolved(*adress, RESOLVE_ADDRESS_TIMEOUT)) {
        case NET_SUCCESS:
            SDL_Log("Succesfully resolved address of: %s\n", NET_GetAddressString(*adress));
            return NET_SUCCESS;

        case NET_FAILURE:
            SDL_Log("Failed to resolve address!\n");
            return NET_FAILURE;

        default:
            SDL_Log("Ops... something went terribly wrong when trying to resolve a network address!\n");
            return NET_FAILURE;

    }

}