#ifndef CLIENTNET_H
#define CLIENTNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"

void createTCPClient(AppState state, int portNumber);

#endif