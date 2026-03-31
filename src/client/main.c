#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#include <SDL3/SDL_main.h>
#include "../lib/player.h" //All dependencies of [x] included

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState* state = (AppState*)SDL_calloc(1, sizeof(AppState)); //Create space on heap
    if(!state) return SDL_APP_FAILURE;

    if(initDisplay(state)) return SDL_APP_FAILURE; //Initiate and display window
    initArt(state);

    state->running = true; //Custom flag to mark the program as running
    state->lastTime = 0;

    // Vector2D tempVec = {50, 50};
    // Stats tempStats = {0};
    // updatePlayer(state->players, 1, tempVec, CLASS_NONE, tempStats, state->renderer);
    // tempVec.x = 100;
    // updatePlayer(state->players, 2, tempVec, CLASS_NONE, tempStats, state->renderer);

    *appstate = state; //Share the appstate to callbacks below
    state->renderFlag = 1;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) //Runs on every event update
{
    AppState* state = (AppState*)appstate;

    return checkEvents(state, event);
}

SDL_AppResult SDL_AppIterate(void *appstate) //Superloop
{
    AppState* state = (AppState*)appstate;

    return render(state);
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) //Runs after returning APP_SUCESS and SDL_FAILURE
{
    if(appstate != NULL) {
        AppState* state = (AppState*)appstate;
        for (int x = 0; x < MAX_PLAYERS; x++) {
            if(state->players[x].texture) SDL_DestroyTexture(state->players[x].texture);
        }
        if(state->renderer) SDL_DestroyRenderer(state->renderer);
        if(state->window) SDL_DestroyWindow(state->window);
        SDL_free(state);
    }

    SDL_Log("Quit done");
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}