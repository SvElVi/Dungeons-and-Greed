#ifndef SERVERNET_H
#define SERVERNET_H

#include <SDL3_net/SDL_net.h>

#define MAX_PLAYERS 5

typedef struct {
    // TCP socket
    NET_StreamSocket *tcpSocket;

    // Client IPs
    NET_Address *clientIPs[MAX_PLAYERS];

} ServerState;

ServerState* createServerState();

#endif