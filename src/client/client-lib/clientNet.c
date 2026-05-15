#include "clientNet.h"
#define TCP_TIMEOUT 5000
#define NET_DEBUG 1
#define LOCAL_PLAYER_ID 0
#define ALLOWED_SERVER_DISTANCE_OUT_OF_SYNC 600 // SQUARED !-! deltaX 300 !-! deltaY 300

void clientNetStateLoop(AppState state)
{
    NETPacket packet, inGameTCPPacket;
    static int counter = 0;
    int ingameTCPFlag = 0;

    switch (state->gameState)
    {
    case GAME_NET_INIT:
        createUDPSocket(state->ptrNetworkInterface, CLIENT_UDP_PORT);
        state->gameState = GAME_IP_INIT;
        break;

    case GAME_IP_INIT:
        if (initAddress(netGetServerIP(state->ptrNetworkInterface), state->hostIP))
            state->gameState = GAME_IP_INIT_CHECK;
        break;

    case GAME_IP_INIT_CHECK:
        switch (NET_GetAddressStatus(*(netGetServerIP(state->ptrNetworkInterface))))
        {
        case NET_SUCCESS:
            state->gameState = GAME_TCP_INIT;
            break;
        }
        break;

    case GAME_TCP_INIT:
        createTCPClient(state->ptrNetworkInterface, SERVER_TCP_PORT);
        state->gameState = GAME_HANDSHAKE;
        break;

    case GAME_HANDSHAKE:
        switch (NET_GetConnectionStatus(netGetStreamSocket(state->ptrNetworkInterface, LOCAL_PLAYER_ID)))
        {
        case NET_SUCCESS:
            clientGameHandshake(state->ptrNetworkInterface);
            state->gameState = GAME_VERIFYING_HANDSHAKE;
            break;

        case NET_FAILURE:
            state->serverState = WAITING_FOR_PLAYERS;
            break;
        }

        break;

    case GAME_VERIFYING_HANDSHAKE:
        if (readTCPData(&packet, netGetStreamSocket(state->ptrNetworkInterface, LOCAL_PLAYER_ID)))
        {

            if (packet.command == APPROVED_PLAYER)
            {
                SDL_Log("Server: You're playerID is: %d\n", packet.PlayerID);
                state->curPlayerPtr = &(state->players[packet.PlayerID]);
                state->curPlayerPtr->playerID = packet.PlayerID;
                packet.command = CONFIRMING_RECIVED_PLAYER_ID;
                sendTCPData(&packet, netGetStreamSocket(state->ptrNetworkInterface, LOCAL_PLAYER_ID));
                state->gameState = GAME_WAITING_FOR_OTHER_PLAYERS;
            }
        }
        break;

    case GAME_WAITING_FOR_OTHER_PLAYERS:
        if (readTCPData(&packet, netGetStreamSocket(state->ptrNetworkInterface, LOCAL_PLAYER_ID)))
        {
            switch (packet.command)
            {
            case UPDATE_WAITING_STATUS:
                state->amountOfPlayers = packet.intData;
                break;

            case SERVER_START_GAME:
                state->seed = packet.intData;
                state->gameState = GAME_GENERATE_WORLD;
                break;
            }
        }
        break;

    case GAME_START:
        state->gameState = GAME_PLAYING;
        break;

    case GAME_PLAYING:
        if (state->onlineMode)
        {
            if (readTCPData(&inGameTCPPacket, netGetStreamSocket(state->ptrNetworkInterface, LOCAL_PLAYER_ID)))
            {
                switch (inGameTCPPacket.command)
                {
                case PLAYER_DEAD:
                    if(inGameTCPPacket.PlayerID == state->curPlayerPtr->playerID)
                    {
                    state->gameState = GAME_DEAD;                        
                    }
                    ingameTCPFlag = 1;
                    break; 
                case SERVER_SHUTDOWN:
                    state->gameState = GAME_SERVER_SHUTDOWN;
                    ingameTCPFlag = 1;
                    break;
                }
                if (ingameTCPFlag)
                    break;
            }

            checkForDatagram(state->ptrNetworkInterface, &packet);
            switch (packet.command)
            {
            case UPDATE_CLIENT_PLAYERS:
                updateClientPlayers(state, &packet);
                break;
            }

            counter++;
            if (counter >= 10000)
            {
                counter = 0;
                state->gameState = GAME_UPDATE_MY_LOCATION;
            }
        }

        break;

    case GAME_UPDATE_MY_LOCATION:
        updateMyLocation(state);
        state->gameState = GAME_PLAYING;
        break;
    }
}

void createTCPClient(NetworkInterface ptrNetworkInterface, int portNumber)
{
    SDL_Log("Initializing a TCP stream socket...\n");
    netSetTCPClient(ptrNetworkInterface, NET_CreateClient(*netGetServerIP(ptrNetworkInterface), portNumber), LOCAL_PLAYER_ID);
}

void clientGameHandshake(NetworkInterface ptrNetworkInterface)
{
    NETPacket packet = {REQUESTING_PLAYER_ID, 0};

    sendTCPData(&packet, netGetStreamSocket(ptrNetworkInterface, LOCAL_PLAYER_ID));
}

void updateMyLocation(AppState state)
{
    const int currentPlayer = state->curPlayerPtr->playerID;
    NETPacket locPacket = {.command = UPDATE_SERVER_PLAYER, .PlayerID = currentPlayer};
    sanitizePlayerStruct(state->curPlayerPtr, &locPacket.players[currentPlayer]);
    sendDatagram(state->ptrNetworkInterface, netGetServerIPForTX(state->ptrNetworkInterface), SERVER_UDP_PORT, &locPacket);
}

void updateClientPlayers(AppState state, NETPacket *packet)
{
    for (int id = 0; id < MAX_PLAYERS; id++)
    {
        if (playerSyncCheck(state, packet) && id == state->curPlayerPtr->playerID)
        {
            continue;
        }
        else
        {
            memcpy(&state->players[id].pos, &packet->players[id].pos, sizeof(Vector2D));         // update position
            memcpy(&state->players[id].flags, &packet->players[id].flags, sizeof(Player_Flags)); // update player flag
            memcpy(&state->players[id].class, &packet->players[id].class, sizeof(Player_Class)); // update player class
            memcpy(&state->players[id].stats, &packet->players[id].stats, sizeof(Stats));        // update stats
            memcpy(&state->players[id].facing, &packet->players[id].facing, sizeof(direction));  // update facing
            memcpy(&state->players[id].flip, &packet->players[id].flip, sizeof(SDL_FlipMode));   // update flip
            // memcpy(&state->players[id].enemyCollisionTimer, &packet->players[id].enemyCollisionTimer, sizeof(Uint32));
            memcpy(&state->players[id].connected, &packet->players[id].connected, sizeof(int)); // update connected
        }
    }
}

bool playerSyncCheck(AppState state, NETPacket *packet)
{
    int deltaX = (state->curPlayerPtr->pos.x - packet->players[state->curPlayerPtr->playerID].pos.x);
    int deltaY = (state->curPlayerPtr->pos.y - packet->players[state->curPlayerPtr->playerID].pos.y);
    /*  if ((deltaX * deltaX) + (deltaY * deltaY) > (ALLOWED_SERVER_DISTANCE_OUT_OF_SYNC * ALLOWED_SERVER_DISTANCE_OUT_OF_SYNC))
     {
         SDL_Log("(PlayerSyncCheck) Resync, prepare for rubber-effect!\n");
     } */

    return true;
}