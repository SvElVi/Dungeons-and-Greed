#define SDL_MAIN_USE_CALLBACKS 1 //Flag to use callbacks
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "inits.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) //Runs once at the begining of the program
{
    SDL_InitSubSystem(SDL_INIT_VIDEO); //Also initilizes appevents

    AppState* state = (AppState*)SDL_calloc(1, sizeof(AppState)); //Create space on heap
    if(!state) return SDL_APP_FAILURE;

    if(!(state->displayID = SDL_GetPrimaryDisplay())) {
        SDL_Log("Failed getting DisplayID: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    } else if(!SDL_GetDisplayBounds(state->displayID, &(state->displaySize))) {
        SDL_Log("Failed getting window bounds: %", SDL_GetError());
        return SDL_APP_FAILURE;
    } else if (!SDL_CreateWindowAndRenderer("GameWindow", state->displaySize.w, state->displaySize.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED, &(state->window), &(state->renderer))) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->running = true; //Custom flag to mark the program as running

    *appstate = state; //Share the appstate to callbacks below

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) //Runs on every event update
{
    AppState* state = (AppState*)appstate;

    const bool* keylist = SDL_GetKeyboardState(0);

    if (keylist[SDL_SCANCODE_ESCAPE] ||
        event->type == SDL_EVENT_QUIT) {
        state->running = false;
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
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