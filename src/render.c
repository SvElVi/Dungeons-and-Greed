#include <SDL3/SDL.h>
#include "inits.h"
#define FRAMES_PER_SECOND 60

void renderFrame(SDL_Renderer** renderer) {
    SDL_SetRenderDrawColor(*renderer,0,255,255,0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void render(AppState* state) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 currentTime = SDL_GetTicks();
    state->deltaTime = state->lastTime - currentTime;
    if(currentTime >= state->lastTime + (1000/FRAMES_PER_SECOND)){ //Should be (&& computedEvent) to avoid dublicate frames which should be set to true after a frame altering event in checkEvents()
        state->lastTime += 1000/FRAMES_PER_SECOND;
        renderFrame(&(state->renderer));
    }

}