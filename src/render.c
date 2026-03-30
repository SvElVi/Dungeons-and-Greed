#include <SDL3/SDL.h>
#include "inits.h"
#define FRAMES_PER_SECOND 60

int renderFrame(AppState* state) {
    int debug;
    SDL_SetRenderDrawColor(state->renderer,0,255,255,0);
    SDL_RenderClear(state->renderer);
    debug = SDL_RenderTexture(state->renderer, state->players[0].texture, NULL, &(state->players[0].renderBox));
    if(!debug) {
        SDL_Log("FAILED RENDERING TEXTURE: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

int render(AppState* state) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 currentTime = SDL_GetTicks();
    state->deltaTime = state->lastTime - currentTime;
    if(currentTime >= state->lastTime + (1000/FRAMES_PER_SECOND)){ //Should be (&& computedEvent) to avoid dublicate frames which should be set to true after a frame altering event in checkEvents()
        state->lastTime += 1000/FRAMES_PER_SECOND;
        if(renderFrame(state)) return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}