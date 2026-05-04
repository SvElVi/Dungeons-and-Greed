#ifndef SERVERNET_H
#define SERVERNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"
#include "../../lib/NET/networkInterface.h"

void createTCPServer(int portNumber, AppState state);

void broadcastTCPToClients(AppState state, NetCommands command, int playerID, int intData);

void updateServerPlayer(AppState state, NETPacket *packet);

#endif