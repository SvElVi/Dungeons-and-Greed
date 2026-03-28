#include <SDL3/SDL.h>
#include "inits.h"

int initDisplay(AppState* state) { //Temporatly here, waiting for potential fix
    if(!(state->displayID = SDL_GetPrimaryDisplay())) {
        SDL_Log("Failed getting DisplayID: %s", SDL_GetError());
        return 1;
    } else if(!SDL_GetDisplayBounds(state->displayID, &(state->displaySize))) {
        SDL_Log("Failed getting window bounds: %", SDL_GetError());
        return 1;
    } else if (!SDL_CreateWindowAndRenderer("GameWindow", state->displaySize.w, state->displaySize.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED, &(state->window), &(state->renderer))) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    return 0;
}