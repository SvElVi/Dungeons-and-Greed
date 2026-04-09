#include "clientNet.h"

void createTCPClient(AppState state, int portNumber) {
    state->tcpClient = NET_CreateClient(NULL, portNumber);

}