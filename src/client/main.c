#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#define SERVER_PORT 2000 // As of now... hardwired...
#define CLIENT_PORT 2001 // As of now... hardwired...

#include <SDL3/SDL_main.h>
#include "../lib/NET/networkInterface.h"
#include "../lib/player.h" //All dependencies of [x] included

int status;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_Log("\n\n --------------- Starting Greedy-Delvers ---------------\n");
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState state = createAppState();
    if(!state) return SDL_APP_FAILURE;

    if(initDisplay(state)) return SDL_APP_FAILURE; //Initiate and display window
    initCam(state);

    if(!startSDLNet()) {
        return SDL_APP_FAILURE;
    }

    state->udpSocket = NET_CreateDatagramSocket(NULL, CLIENT_PORT);

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
    renderDungeon(state);

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

    int message = 1337;

    NET_Address *adr;

    // Skriv in din lokala IP-adress här
    // Om du ej vet...
    // Sök på CMD i sökfältet, skriv in ipconfig
    // så hittar du den bredvid IPv4 address 
    adr = NET_ResolveHostname("127.0.0.1");

    status = NET_GetAddressStatus(adr);

    // Det tar tid att "resolva" en address, så vi måste kolla att det är klart...
    // Detta är inte blockerande, utan en pollad lösning
    switch(status) {
        case NET_SUCCESS:
            NET_SendDatagram(state->udpSocket, adr, SERVER_PORT, (void *)&message, sizeof(message));
            break;

        default:
            break;
    }

    // Det verkar handla om tid... vi kan inte göra något innan allt är redo
    if(NET_ReceiveDatagram(state->udpSocket, state->udpPacket)) {
        if (status) {
            if ((*state->udpPacket)!= NULL) {
                int test;
                // Kopierar över data
                memccpy(&test, (*state->udpPacket)->buf, 1, sizeof((*state->udpPacket)->buf));
                SDL_Log("Vi fick data, och den är: %d\n", test);
                (*state->udpPacket) = NULL;
            }
        }
    }

    return render(state);
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) //Runs after returning APP_SUCESS or SDL_FAILURE
{
    if(appstate != NULL) {
        AppState state = (AppState)appstate;
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