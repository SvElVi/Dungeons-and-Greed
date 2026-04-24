#define SDL_MAIN_USE_CALLBACKS 1 // Flag to use callbacks
#define TCP_PORT 2000
#define UDP_PORT 2020

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

    createUDPSocket(&state->udpSocket, UDP_PORT);

    state->udpPacket = SDL_calloc(1, sizeof(NET_Datagram));

    state->running = true; // Custom flag to mark the program as running

    Vector2D tempVec = {0, 0};
    Stats fullHp = {100, 100};
    Stats halfHp = {50, 100};
    Stats smallHp = {10, 100};
    updatePlayer(&(state->players[0]), tempVec, CLASS_NONE, fullHp, state->renderer);
    SDL_strlcpy(state->players[0].name, "Player1", sizeof(state->players[0].name));
    tempVec.x = 120;
    updatePlayer(&(state->players[1]), tempVec, CLASS_NONE, halfHp, state->renderer);
    SDL_strlcpy(state->players[1].name, "Player2", sizeof(state->players[1].name));
    tempVec.x = 240;
    updatePlayer(&(state->players[2]), tempVec, CLASS_NONE, smallHp, state->renderer);
    SDL_strlcpy(state->players[2].name, "Player3", sizeof(state->players[2].name));

    // enemy
    Vector2D enemyPos = {200, 100};
    Stats enemyStats = {100, 100, 0, 5, 10, 1};
    updateEnemy(&state->enemies[0], enemyPos, ENEMY_SKELETON, enemyStats, state->renderer);
    *appstate = state; // Share the appstate to callbacks below
    // state->renderFlag = 1;

    state->world = createWorld(5, (Uint64)SDL_rand(0), state->renderer);

    createDungeon(state->world, 20);

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
    void *rxData, *txData;

    switch (state->serverState)
    {
    case INIT_OF_SERVER:
        createTCPServer(TCP_PORT, state);
        state->serverState = WAITING_FOR_PLAYERS;
        break;

    case WAITING_FOR_PLAYERS:
        if (NET_AcceptClient(state->tcpServer, &state->serverStreamSocket))
        {
            if (state->serverStreamSocket != NULL)
            {
                SDL_Log("Hittade en spelare med IP: %s\n", NET_GetAddressString(NET_GetStreamSocketAddress(state->serverStreamSocket)));
                state->serverState = ASSIGNING_PLAYER_ID;
            }
        }

        break;

    case ASSIGNING_PLAYER_ID:
        if (NET_ReadFromStreamSocket(state->serverStreamSocket, rxData, sizeof(NETPacket)) > 0)
        {
            switch ((*(NETPacket *)rxData).command)
            {
            case REQUESTING_PLAYER_ID:
                SDL_Log("Förfrågan om PlayerID från klient: %s\n", NET_GetAddressString(NET_GetStreamSocketAddress(state->serverStreamSocket)));
                state->serverState = SENDING_PLAYER_ID;
                break;

            default:
                SDL_Log("Incorrect command during player assignment!\n");
                state->serverState = WAITING_FOR_PLAYERS;
            }
        }
        break;

    case SENDING_PLAYER_ID:
        packet.command = APPROVED_PLAYER;
        packet.PlayerID = state->connectedPlayers.amountOfPlayers;
        SDL_Log("Skickar PlayerID %d till klient: %s\n", packet.PlayerID, NET_GetAddressString(NET_GetStreamSocketAddress(state->serverStreamSocket)));

        txData = &packet;
        NET_WriteToStreamSocket(state->serverStreamSocket, txData, sizeof(NETPacket));
        state->serverState = CONFIRMING_PLAYER_ID_RECIVE;

        break;

    case CONFIRMING_PLAYER_ID_RECIVE:
        if (NET_ReadFromStreamSocket(state->serverStreamSocket, rxData, sizeof(NETPacket)) > 0)
        {
            switch ((*(NETPacket *)rxData).command)
            {
            case CONFIRMING_RECIVED_PLAYER_ID:
                if ((*(NETPacket *)rxData).PlayerID != state->connectedPlayers.amountOfPlayers)
                {
                    SDL_Log("PlayerID error during confirmation!\n");
                    state->serverState = WAITING_FOR_PLAYERS;
                }
                else
                {
                    if (state->connectedPlayers.amountOfPlayers >= MAX_PLAYERS)
                    {
                        NET_DestroyStreamSocket(state->serverStreamSocket);
                        state->serverState = STARTING_GAME;
                    }
                    else
                    {
                        state->serverState = WAITING_FOR_PLAYERS;
                        state->connectedPlayers.amountOfPlayers++;
                        NET_DestroyStreamSocket(state->serverStreamSocket);
                        SDL_Log("Waiting for players: %d/5\n", state->connectedPlayers.amountOfPlayers);
                    }
                }
                break;

            default:
                SDL_Log("Incorrect command during player assignment!\n");
                state->serverState = WAITING_FOR_PLAYERS;
            }
        }
        break;

    case STARTING_GAME:
        SDL_Log("STARTING GAME!\n");
        break;

    case GAME_ONGOING:

        break;

    case SERVER_CLEANUP:

        break;
    }

    return render(state);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) // Runs after returning APP_SUCESS or SDL_FAILURE
{
    if (appstate != NULL)
    {
        AppState state = (AppState)appstate;

        destoryUDPSocket(state->udpSocket);
        SDL_free(state->udpPacket);
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

    SDL_Log("Quit done");
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}