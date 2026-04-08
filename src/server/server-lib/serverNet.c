#include "serverNet.h"

#define MAX_PLAYERS 5

typedef struct {
    // TCP socket
    NET_StreamSocket *tcpSocket;

    // Client IPs
    NET_Address *clientIPs[MAX_PLAYERS];

} serverState;