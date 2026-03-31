#include "networkInterface.h"

enum {
    FAIL,
    SUCCESS
};

int startSDLNet(void);
NET_Server* startServer(int);

int startSDLNet(void) {
    SDL_Log("Initializing SDL_Net...");
    if(NET_Init()) {
        SDL_Log("Succesfully started SDL_Net!");
        return SUCCESS;
        
    } else {
        SDL_Log("Fatal error: SDL_Net failed to start!");
        return FAIL;

    }
}

NET_Server* startServer(int portNumber) {
    return NET_CreateServer(NULL, portNumber);
}