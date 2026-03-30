#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#include <SDL3/SDL_main.h>
#include "render.h" //All dependencies of render.h included

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState* state = (AppState*)SDL_calloc(1, sizeof(AppState)); //Create space on heap
    if(!state) return SDL_APP_FAILURE;

    if(initDisplay(state)) return SDL_APP_FAILURE; //Initiate and display window

    state->running = true; //Custom flag to mark the program as running
    state->lastTime = 0;

    *appstate = state; //Share the appstate to callbacks below

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

    render(state);

    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) //Runs after returning APP_SUCESS and SDL_FAILURE
{
    if(appstate != NULL) {
        AppState* state = (AppState*)appstate;
        for (int x = 0; x < MAX_PLAYERS; x++) {
            if(state->clients[x].player.texture) SDL_DestroyTexture(state->clients[x].player.texture);
        }
        if(state->renderer) SDL_DestroyRenderer(state->renderer);
        if(state->window) SDL_DestroyWindow(state->window);
        SDL_free(state);
    }

    SDL_Log("Quit done");
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}