#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "events.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState* state = (AppState*)SDL_calloc(1, sizeof(AppState)); //Create space on heap
    if(!state) return SDL_APP_FAILURE;

    if(initDisplay(state)) return SDL_APP_FAILURE; //Initiate and display window

    state->running = true; //Custom flag to mark the program as running

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

    SDL_SetRenderDrawColor(state->renderer,0,255,255,0);
    SDL_RenderClear(state->renderer);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) //Runs after returning APP_SUCESS and SDL_FAILURE
{
    if(appstate != NULL) {
        AppState* state = (AppState*)appstate;
        if(state->renderer) SDL_DestroyRenderer;
        if(state->window) SDL_DestroyWindow(state->window);
        SDL_free(state);
    }

    SDL_Log("Quit done");
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}