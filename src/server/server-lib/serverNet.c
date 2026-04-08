#include "serverNet.h"

ServerState* createServerState();

ServerState* createServerState() {
    ServerState *ptrSeverState = SDL_calloc(1, sizeof(ServerState));
    return ptrSeverState;
}