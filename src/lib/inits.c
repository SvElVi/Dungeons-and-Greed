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

    SDL_SetDefaultTextureScaleMode(state->renderer, SDL_SCALEMODE_PIXELART);

    return SDL_APP_CONTINUE;
}

void initArt(AppState* state) { //Init before the game, seperate event should be used later upon a class selection for the player
    SDL_Surface* pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
    state->players[0].texture = SDL_CreateTextureFromSurface(state->renderer, pArt);
    SDL_DestroySurface(pArt);
    state->camera.x = (state->displayMode->w - SPRITE_SIZE * RENDER_SCALE)/2;
    state->camera.y = (state->displayMode->h - SPRITE_SIZE * RENDER_SCALE)/2;
    state->camera.w = SPRITE_SIZE * RENDER_SCALE;
    state->camera.h = SPRITE_SIZE * RENDER_SCALE;
}

void updatePlayer() {
    
}