#include "player.h"

int renderFrame(AppState* state) {
    SDL_FRect temp;
    SDL_SetRenderDrawColor(state->renderer,0,255,255,0);
    SDL_RenderClear(state->renderer);

    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(state->players[i].texture) {

            temp.h = PLAYER_SIZE*PLAYER_RENDER_SCALE;
            temp.w = PLAYER_SIZE*PLAYER_RENDER_SCALE;
            temp.x = state->camera.x + (state->players[0].pos.x - state->players[i].pos.x);
            temp.y = state->camera.y + (state->players[0].pos.y - state->players[i].pos.y);

            if(!(SDL_RenderTexture(state->renderer, state->players[i].texture, &(state->players[i].aniBox), &(temp)))) {
                SDL_Log("FAILED RENDERING TEXTURE: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        }
    }

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

int render(AppState* state) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 currentTime = SDL_GetTicks();
    if(currentTime >= state->lastTime + (1000/state->framerate)){ //Should be (&& computedEvent) to avoid dublicate frames which should be set to true after a frame altering event in checkEvents()
        state->deltaTime = currentTime - state->lastTime;
        state->lastTime = currentTime;
        if(state->renderFlag) {
            //should be moved to a seperate callback function
            movement(&(state->players[0]), state->deltaTime);
            playerAnimate(&(state->players[0]), &(state->animationTime));
            SDL_Log("TIMER: %d", state->animationTime);

            !state->renderFlag;
            if(renderFrame(state)) return SDL_APP_FAILURE;
        }
    }
    return SDL_APP_CONTINUE;
}