#ifndef CLIENTNET_H
#define CLIENTNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"
#include "../../lib/player.h"
#include "../lib/NET/networkInterface.h"

void createTCPClient(NetworkInterface ptrNetworkInterface, int portNumber);

void clientGameHandshake(NetworkInterface ptrNetworkInterface);

void clientNetStateLoop(AppState state);

void updateMyLocation(AppState state);

void updateClientPlayers(AppState state, NETPacket *packet);

bool playerSyncCheck(AppState state, NETPacket *packet);

#endif