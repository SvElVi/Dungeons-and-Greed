#ifndef CLIENTNET_H
#define CLIENTNET_H

#include <SDL3_net/SDL_net.h>
#include "../../lib/inits.h"
#include "../lib/NET/networkInterface.h"

// Takes a NET_Address, portnumber and the AppState, where the address + port is the remote host,
// and AppState is just used to store the streamsocket.
// Note that the NET_Address must be resolved before calling this function!
// THIS FUNCTION IS BLOCKING, WITH A TIMEOUT OF 1000 MS!
void createTCPClient(NET_Address *adr, int portNumber, AppState state);

NET_Status checkStreamsocketConnection(AppState state);

void clientTCPHandshake(AppState state, NET_StreamSocket *streamSocket);

#endif