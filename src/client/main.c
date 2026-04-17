#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#define SERVER_PORT 2000 // As of now... hardwired...
#define CLIENT_PORT 2001 // As of now... hardwired...
#define DEBUG 1

#include <SDL3/SDL_main.h>
#include "../lib/NET/networkInterface.h"
#include "client-lib/clientNet.h"
#include "../lib/player.h" //All dependencies of [x] included

char ip[15] = "127.0.0.1";

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_Log("\n\n --------------- Starting Greedy-Delvers ---------------\n");
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState state = createAppState();
    state->gameState = GAME_INIT;
    if(!state) return SDL_APP_FAILURE;

    state->gameState = GAME_START;

    if(initDisplay(state)) return SDL_APP_FAILURE; //Initiate and display window
    initCam(state);

    if(!startSDLNet()) {
        return SDL_APP_FAILURE;
    }

    createUDPSocket(&state->udpSocket, SERVER_PORT);

    state->udpPacket = SDL_calloc(1, sizeof(NET_Datagram));

    state->running = true; //Custom flag to mark the program as running

    Vector2D tempVec = {0, 0};
    Stats tempStats = {0};
    updatePlayer(&(state->players[0]), tempVec, CLASS_NONE, tempStats, state->renderer);
    tempVec.x = 50;
    updatePlayer(&(state->players[1]), tempVec, CLASS_NONE, tempStats, state->renderer);
    tempVec.x = 100;
    updatePlayer(&(state->players[2]), tempVec, CLASS_NONE, tempStats, state->renderer);

    *appstate = state; //Share the appstate to callbacks below
    // state->renderFlag = 1;

    state->world = createWorld(5, (Uint64)SDL_rand(0), state->renderer);

    createDungeon(state->world, 20);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) //Runs on every event update
{
    AppState state = (AppState)appstate;

    return checkEvents(state, event);
}

SDL_AppResult SDL_AppIterate(void *appstate) //Superloop
{
    AppState state = (AppState)appstate;
    
    switch (state->gameState) {
        case GAME_INIT:
            state->gameState = GAME_MENY;
            break;

        case GAME_MENY:
            // Meny passar bra att fixa här, oavsett vart i koden den ligger...
            state->gameState = GAME_TCP_INIT;
            break;

        case GAME_TCP_INIT:
            if (initAddress(&state->serverIP, ip) < 0) {
                createTCPClient(state->serverIP, SERVER_PORT, state);
            }
            state->gameState = GAME_TCP_HANDSHAKE;
            break;

        case GAME_TCP_HANDSHAKE:
            if (checkStreamsocketConnection(state) == NET_SUCCESS) {
                clientTCPHandshake(state);
                state->gameState = GAME_TCP_VERIFYING_HANDSHAKE;
            }
            break;

        case GAME_TCP_VERIFYING_HANDSHAKE:
            if (handshakeDone(state)) state->gameState = GAME_START;
            break;

        case GAME_START:
            state->gameState = GAME_PLAYING;
            break;

        case GAME_PLAYING:
            break;

        case GAME_OVER:
            break;

        default:
            break;

    }

    return render(state);
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) //Runs after returning APP_SUCESS or SDL_FAILURE
{
    if(appstate != NULL) {
        AppState state = (AppState)appstate;

        destoryUDPSocket(state->udpSocket);
        stopSDLNet();

        for (int x = 0; x < MAX_PLAYERS; x++) {
            if(state->players[x].texture) SDL_DestroyTexture(state->players[x].texture);
        }
        if(state->renderer) SDL_DestroyRenderer(state->renderer);
        if(state->window) SDL_DestroyWindow(state->window);
        destroyWorld(state->world);
        SDL_free(state);
    }

    SDL_Log("Quit done");
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}