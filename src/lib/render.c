#include "player.h"
#include <string.h>

int renderFrame(AppState state) {
    SDL_FRect temp;
    Vector2D tempV, renderOrder[MAX_PLAYERS];
    SDL_SetRenderDrawColor(state->renderer,37,19,26,1);
    SDL_RenderClear(state->renderer);

    if(!renderDungeon(state)) {
        return SDL_APP_FAILURE;
    }

    for(int j = 0; j < MAX_PLAYERS; j++) {
        renderOrder[j].x = j;
        renderOrder[j].y = state->players[j].pos.y;
    }

    for (int a = 0; a < MAX_PLAYERS -1; a++) {
        for (int b = 0; b < MAX_PLAYERS -1 - a; b++) {
            if (renderOrder[b].y < renderOrder[b+1].y) {
                tempV = renderOrder[b];
                renderOrder[b] = renderOrder[b+1];
                renderOrder[b+1] = tempV;
            }
        }
    }

    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(state->players[renderOrder[i].x].texture) {

            temp.h = PLAYER_SIZE*RENDER_SCALE;
            temp.w = PLAYER_SIZE*RENDER_SCALE;
            temp.x = state->camera.x + (state->players[0].pos.x - state->players[renderOrder[i].x].pos.x);
            temp.y = state->camera.y + (state->players[0].pos.y - state->players[renderOrder[i].x].pos.y);

            if(!(SDL_RenderTextureRotated(state->renderer, state->players[renderOrder[i].x].texture, &(state->players[renderOrder[i].x].aniBox), &(temp), 0, NULL, state->players[renderOrder[i].x].flip))) {
                SDL_Log("FAILED RENDERING TEXTURE: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        }
    }

    if(state->gameState == GAME_START){
        const char *message = "Press SPACE to start";
        int w = 0, h = 0;
        float x, y;
        const float scale = 4.0f;

        /* Center the message and scale it up */
        SDL_GetRenderOutputSize(state->renderer, &w, &h);
        SDL_SetRenderScale(state->renderer, scale, scale);
        x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
        y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

        /* Draw the message */
        SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(state->renderer, x, y, message);
        SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
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
        if(state->gameState == GAME_PLAYING){
            movement(&(state->players[0]), state->players, state->deltaTime);
            animatePlayers(state->players, &(state->animationTime), state->framerate, &(state->computedEvent));
        }
        

        if(renderFrame(state)) return SDL_APP_FAILURE;
        // }
    }
    return SDL_APP_CONTINUE;
}