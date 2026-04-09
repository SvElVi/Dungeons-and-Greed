#include "serverNet.h"

void createTCPServer(AppState state, int portNumber) {
    state->tcpServer = NET_CreateServer(NULL, portNumber);

}