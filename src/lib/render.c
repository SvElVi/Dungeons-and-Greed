#include "player.h"

int renderFrame(AppState state) {
    SDL_FRect temp;
    SDL_SetRenderDrawColor(state->renderer,0,255,255,0);
    SDL_RenderClear(state->renderer);

    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(state->players[i].texture) {

            temp.h = PLAYER_SIZE*PLAYER_RENDER_SCALE;
            temp.w = PLAYER_SIZE*PLAYER_RENDER_SCALE;
            temp.x = state->camera.x + (state->players[0].pos.x - state->players[i].pos.x);
            temp.y = state->camera.y + (state->players[0].pos.y - state->players[i].pos.y);

            if(!(SDL_RenderTextureRotated(state->renderer, state->players[i].texture, &(state->players[i].aniBox), &(temp), 0, NULL, state->players[i].flip))) {
                SDL_Log("FAILED RENDERING TEXTURE: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        }
    }

    SDL_RenderPresent(state->renderer);
    state->computedEvent = false;

    return SDL_APP_CONTINUE;
}

int render(AppState state) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 currentTime = SDL_GetTicks();
    if(currentTime >= state->lastTime + (1000/state->framerate)){ //renderflag unused
        state->deltaTime = currentTime - state->lastTime;
        state->lastTime = currentTime;
        // if(state->renderFlag) {
        movement(&(state->players[0]), state->deltaTime);
        animatePlayers(state->players, &(state->animationTime), state->framerate, &(state->computedEvent));

        if(renderFrame(state)) return SDL_APP_FAILURE;
        // }
    }
    return SDL_APP_CONTINUE;
}