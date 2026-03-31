#include "networkInterface.h"
#include <SDL3_Net/SDL_net.h>

enum {
    FAIL,
    SUCCESS
};

int startSDLNet(void);

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