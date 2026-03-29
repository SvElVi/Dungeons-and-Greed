#include <SDL3/SDL.h>
#define FRAMES_PER_SECOND 60

void renderFrame(SDL_Renderer** renderer) {
    SDL_SetRenderDrawColor(*renderer,0,255,255,0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void render(Uint64* lTime, Uint64* dTime, SDL_Renderer** renderer) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 cTime = SDL_GetTicks();
    *dTime = *lTime - cTime;
    if(cTime >= *lTime + (1000/FRAMES_PER_SECOND)){ //Should be (&& computedEvent) to avoid dublicate frames which should be set to true after a frame altering event in checkEvents()
        *lTime += 1000/FRAMES_PER_SECOND;
        renderFrame(renderer);
    }

}