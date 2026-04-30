#ifndef SERVERNET_H
#define SERVERNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"
#include "../../lib/NET/networkInterface.h"

// Takes a NET_Address, portnumber and the AppState, where the address + port is the remote host,
// and AppState is just used to store the streamsocket.
// Note that the NET_Address must be resolved before calling this function!
void createTCPServer(int portNumber, AppState state);

void broadcastToClients(AppState state, NetCommands command, int playerID, int intData);

void updatePlayerLocation(AppState state, PlayerLocations *loc, int playerID);

#endif