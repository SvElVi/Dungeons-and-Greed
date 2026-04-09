#ifndef SERVERNET_H
#define SERVERNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"

#define MAX_PLAYERS 5

void createTCPServer(AppState state, int portNumber);

#endif