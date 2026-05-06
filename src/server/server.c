#define SDL_MAIN_USE_CALLBACKS 1 // Flag to use callbacks

#define DEBUG 0
#define NET_DEBUG 1

#include <SDL3/SDL_main.h>
#include "../lib/NET/networkInterface.h"
#include "server-lib/serverNet.h"
#include "../lib/player.h" //All dependencies of [x] included
#include "../lib/enemy.h"  //All dependencies of [x] included

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) // Runs once at the begining of the program
{
    SDL_Log("\n\n --------------- Starting Greedy-Delvers ---------------\n");
    SDL_InitSubSystem(SDL_INIT_VIDEO); // Also initilizes appevents

    AppState state = createAppState();
    state->connectedPlayers.amountOfPlayers = 0;
    state->gameState = GAME_INIT;
    state->mainMenu = (Menu){
        .menuOptions = {"Join", "Play offline", "Quit"},
        .selected = 0,
        .count = 3};
    if (!state)
        return SDL_APP_FAILURE;

    state->serverState = INIT_OF_SERVER;
    state->gameState = SERVER;

    if (initDisplay(state))
        return SDL_APP_FAILURE; // Initiate and display window
    initCam(state);

    if (startSDLNet() == NET_FAILURE)
        return SDL_APP_FAILURE;

    createUDPSocket(&state->udpSocket, SERVER_UDP_PORT);

    state->udpPacket = SDL_calloc(1, sizeof(NET_Datagram));

    state->running = true; // Custom flag to mark the program as running

    Vector2D tempVec = {0, 0};
    Stats fullHp = {100, 100};
    Stats halfHp = {50, 100};
    Stats smallHp = {10, 100};

    Stats defaultHp = {100, 100};
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Vector2D tempVec = {0, 0};
        char name[PLAYER_NAME_MAX];
        SDL_snprintf(name, sizeof(name), "Player%d", i + 1);
        updatePlayer(&(state->players[i]), tempVec, CLASS_NONE, defaultHp, state->renderer);
        SDL_strlcpy(state->players[i].name, name, sizeof(state->players[i].name));
        state->players[i].connected = false;
    }

    // enemy
    Vector2D enemyPos = {200, 100};
    Stats enemyStats = {100, 100, 0, 5, 10, 1};
    updateEnemy(&state->enemies[0], enemyPos, ENEMY_SKELETON, enemyStats, state->renderer);
    *appstate = state; // Share the appstate to callbacks below
    // state->renderFlag = 1;

    state->world = createWorld(5, 12345, state->renderer); //(Uint64)SDL_rand(0)

    createDungeon(state->world, 20, state, 1);

    // SDL_HideWindow(state->window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) // Runs on every event update
{
    AppState state = (AppState)appstate;

    return checkEvents(state, event);
}

SDL_AppResult SDL_AppIterate(void *appstate) // Superloop
{
    AppState state = (AppState)appstate;
    NETPacket packet;
    int currentPlayer = state->connectedPlayers.amountOfPlayers;
    void *rxData, *txData, *udpTX, *udpRX;
    static bool hasAnnounceAmountOfPlayers = false;
    static int counter = 0;

    switch (state->serverState)
    {
    case INIT_OF_SERVER:
        createTCPServer(SERVER_TCP_PORT, state);
        state->serverState = WAITING_FOR_PLAYERS;
        break;

    case WAITING_FOR_PLAYERS:
        if (!hasAnnounceAmountOfPlayers)
        {
            SDL_Log("Waiting for players: %d/%d\n", state->connectedPlayers.amountOfPlayers, MAX_PLAYERS);
            hasAnnounceAmountOfPlayers = true;
        }
        if (NET_AcceptClient(state->tcpServer, &state->connectedPlayers.tcpClient[currentPlayer]))
        {
            if (state->connectedPlayers.tcpClient[currentPlayer] != NULL)
            {
                hasAnnounceAmountOfPlayers = false;
                SDL_Log("\n---------- TCP handshake ----------\n");
                SDL_Log("Incoming connection from: %s\n", NET_GetAddressString(NET_GetStreamSocketAddress(state->connectedPlayers.tcpClient[currentPlayer])));
                state->serverState = ASSIGNING_PLAYER_ID;
            }
        }

        break;

    case ASSIGNING_PLAYER_ID:
        if (readTCPData(state, &packet, state->connectedPlayers.tcpClient[currentPlayer]))
        {
            switch (packet.command)
            {
            case REQUESTING_PLAYER_ID:
                SDL_Log("Client is requesting a playerID!\n");
                state->serverState = SENDING_PLAYER_ID;
                break;

            default:
                SDL_Log("Error: Client out of sync, not requesting a playerID during ASSIGNING_PLAYER_ID state!\n");
                state->serverState = WAITING_FOR_PLAYERS;
            }
        }
        break;

    case SENDING_PLAYER_ID:
        packet.command = APPROVED_PLAYER;
        packet.PlayerID = currentPlayer;
        SDL_Log("Assigning the client playerID %d, and sending it to: %s\n", packet.PlayerID, NET_GetAddressString(NET_GetStreamSocketAddress(state->connectedPlayers.tcpClient[currentPlayer])));

        sendTCPData(state, &packet, state->connectedPlayers.tcpClient[currentPlayer]);
        state->serverState = CONFIRMING_PLAYER_ID_RECIVE;

        break;

    case CONFIRMING_PLAYER_ID_RECIVE:
        if (readTCPData(state, &packet, state->connectedPlayers.tcpClient[currentPlayer]))
        {
            switch (packet.command)
            {
            case CONFIRMING_RECIVED_PLAYER_ID:
                if (packet.PlayerID != state->connectedPlayers.amountOfPlayers)
                {
                    SDL_Log("Error: Client and server playerID out of sync!\n");
                    state->serverState = WAITING_FOR_PLAYERS;
                }
                else
                {
                    packet.command = UPDATE_WAITING_STATUS;
                    packet.intData = ++state->connectedPlayers.amountOfPlayers;
                    broadcastTCPToClients(state, &packet);
                    if (state->connectedPlayers.amountOfPlayers >= MAX_PLAYERS)
                    {
                        updateServerPlayerIP(state, packet.PlayerID, state->connectedPlayers.tcpClient[currentPlayer]);
                        SDL_Log("-----------------------------------\n\n");
                        SDL_Log("Players as of now:\n\n");
                        for (int index = 0; index < state->connectedPlayers.amountOfPlayers; index++)
                        {
                            SDL_Log("Player %d, have the IP: %s\n", index, NET_GetAddressString(state->connectedPlayers.players[index].ipAddress));
                        }
                        SDL_Log("\n\nStarting game!\n");
                        state->serverState = STARTING_GAME;
                    }
                    else
                    {
                        updateServerPlayerIP(state, packet.PlayerID, state->connectedPlayers.tcpClient[currentPlayer]);
                        state->serverState = WAITING_FOR_PLAYERS;
                        SDL_Log("-----------------------------------\n\n");
                        SDL_Log("Players as of now:\n\n");
                        for (int index = 0; index < state->connectedPlayers.amountOfPlayers; index++)
                        {
                            SDL_Log("Player %d, have the IP: %s\n", index, NET_GetAddressString(state->connectedPlayers.players[index].ipAddress));
                        }
                        SDL_Log("\n\n");
                    }
                }
                break;

            default:
                SDL_Log("Error: Client out of sync, not confirming a playerID during CONFIRMING_PLAYER_ID_RECIVE!\n");
                state->serverState = WAITING_FOR_PLAYERS;
            }
        }
        break;

    case STARTING_GAME:
        packet.command = SERVER_START_GAME;
        packet.uint64 = SDL_rand(0);
        broadcastTCPToClients(state, &packet);
        state->serverState = GAME_ONGOING;
        break;

    case GAME_ONGOING:
        for (int index = 0; index < MAX_PLAYERS; index++)
        {
            checkForDatagram(state, &packet);
            switch (packet.command)
            {
            case UPDATE_SERVER_PLAYER:
                updateServerPlayer(state, &packet);
                state->serverState = BROADCASTING_PLAYERS_TO_CLIENTS;
            }
        }
        break;

    case BROADCASTING_PLAYERS_TO_CLIENTS:
        NETPacket broadcastPacket = {.command = UPDATE_CLIENT_PLAYERS, .intData = 2};

        makeBroadcastPacket(state, &broadcastPacket);

        for (int i = 0; i < state->connectedPlayers.amountOfPlayers; i++)
        {
            broadcastPacket.PlayerID = i;
            sendDatagram(state, state->connectedPlayers.players[i].ipAddress, CLIENT_UDP_PORT, &broadcastPacket);
        }
        state->serverState = GAME_ONGOING;
        break;
    }
    return render(state, &state->connectedPlayers.players[0]);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) // Runs after returning APP_SUCESS or SDL_FAILURE
{
    if (appstate != NULL)
    {
        AppState state = (AppState)appstate;
        NETPacket packet;
        packet.command = SERVER_SHUTDOWN;

        broadcastTCPToClients(state, &packet);

        destoryUDPSocket(state->udpSocket);
        SDL_free(state->udpPacket);
        NET_DestroyServer(state->tcpServer);
        for (int index = 0; index < state->connectedPlayers.amountOfPlayers; index++)
        {
            NET_DestroyStreamSocket(state->connectedPlayers.tcpClient[index]);
        }
        stopSDLNet();

        for (int x = 0; x < MAX_PLAYERS; x++)
        {
            if (state->players[x].texture)
                SDL_DestroyTexture(state->players[x].texture);
        }
        if (state->renderer)
            SDL_DestroyRenderer(state->renderer);
        if (state->window)
            SDL_DestroyWindow(state->window);
        destroyWorld(state->world);
        SDL_free(state);
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}