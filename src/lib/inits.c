#include <SDL3/SDL.h>
#include "inits.h"
#define FALLBACK_FRAMES_PER_SECOND 60

int initDisplay(AppState* state) {
    if(!(state->displayID = SDL_GetPrimaryDisplay())) {
        SDL_Log("Failed getting DisplayID: %s", SDL_GetError());
        return 1;
    } else if(!(state->displayMode = SDL_GetCurrentDisplayMode(state->displayID))) {
        SDL_Log("Failed getting Displaymode: %s", SDL_GetError());
        return 1;
    }else if (!SDL_CreateWindowAndRenderer("GameWindow", state->displayMode->w, state->displayMode->h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED, &(state->window), &(state->renderer))) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    if(state->displayMode->refresh_rate) {
        state->framerate = state->displayMode->refresh_rate;
    } else {
        state->framerate = FALLBACK_FRAMES_PER_SECOND;
    }

    for(int i = 0; i < MAX_PLAYERS; i++) state->players[i].animationTime = 0;

    SDL_SetDefaultTextureScaleMode(state->renderer, SDL_SCALEMODE_PIXELART);

    return SDL_APP_CONTINUE;
}

void initCam(AppState* state) { //Init before the game, seperate event should be used later upon a class selection for the player
    state->camera.x = (state->displayMode->w - PLAYER_SIZE * PLAYER_RENDER_SCALE)/2;
    state->camera.y = (state->displayMode->h - PLAYER_SIZE * PLAYER_RENDER_SCALE)/2;
    state->camera.w = PLAYER_SIZE * PLAYER_RENDER_SCALE;
    state->camera.h = PLAYER_SIZE * PLAYER_RENDER_SCALE;
}