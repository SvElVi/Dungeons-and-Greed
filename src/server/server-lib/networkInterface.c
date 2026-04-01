#include "networkInterface.h"

enum {
    FAIL,
    SUCCESS
};

int startSDLNet(void);
NET_Server* startServer(int);

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

NET_Server* startServer(int portNumber) {
    SDL_Log("Starting server...\n");
    return NET_CreateServer(NULL, portNumber);
}