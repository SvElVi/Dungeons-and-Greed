#define SDL_MAIN_USE_CALLBACKS 1 // Flag to use callbacks
#define TCP_PORT 2000
#define UDP_PORT 2020
#define DEBUG 1

#include <SDL3/SDL_main.h>
#include "../lib/NET/networkInterface.h"
#include "client-lib/clientNet.h"
#include "../lib/player.h" //All dependencies of [x] included
#include "../lib/enemy.h"  //All dependencies of [x] included
#include "../lib/world/world.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) // Runs once at the begining of the program
{
    SDL_Log("\n\n --------------- Starting Greedy-Delvers ---------------\n");
    SDL_InitSubSystem(SDL_INIT_VIDEO); // Also initilizes appevents

    AppState state = createAppState();
    state->gameState = GAME_INIT;
    state->mainMenu = (Menu){
        .menuOptions = {"Play", "Join", "Quit"},
        .selected = 0,
        .count = 3};
    if (!state)
        return SDL_APP_FAILURE;

    if (initDisplay(state))
        return SDL_APP_FAILURE; // Initiate and display window
    initCam(state);

    if (startSDLNet() == NET_FAILURE)
        return SDL_APP_FAILURE;

    state->udpPacket = SDL_calloc(1, sizeof(NET_Datagram));

    state->running = true; // Custom flag to mark the program as running
    state->players[0].classLock = false;

    state->curPlayerPtr = &(state->players[0]); //In case not in multiplayer

    Vector2D tempVec = {0, 0};
    Stats fullHp = {100, 100};
    Stats halfHp = {50, 100};
    Stats smallHp = {10, 100};
    updatePlayer(&(state->players[0]), tempVec, CLASS_NONE, fullHp, state->renderer);
    SDL_strlcpy(state->players[0].name, "Player1", sizeof(state->players[0].name));
    updatePlayer(&(state->players[1]), tempVec, CLASS_NONE, halfHp, state->renderer);
    SDL_strlcpy(state->players[1].name, "Player2", sizeof(state->players[1].name));
    updatePlayer(&(state->players[2]), tempVec, CLASS_NONE, smallHp, state->renderer);
    SDL_strlcpy(state->players[2].name, "Player3", sizeof(state->players[2].name));

    *appstate = state; // Share the appstate to callbacks below

    state->world = createWorld(5, (Uint64)SDL_rand(0), state->renderer);

    createDungeon(state->world, 20, state, 1);

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

    //"Superloop" of net is placed in clientNet.c
    clientNetStateLoop(state);

    return render(state, state->curPlayerPtr);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) // Runs after returning APP_SUCESS or SDL_FAILURE
{
    if (appstate != NULL)
    {
        AppState state = (AppState)appstate;

        destoryUDPSocket(state->udpSocket);
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