// #include <SDL3/SDL.h>
// #include "inits.h"
#include "player.h"

int renderFrame(AppState* state) {
SDL_SetRenderDrawColor(state->renderer, 0, 255, 255, 0);
    SDL_RenderClear(state->renderer);

    Uint32 now = SDL_GetTicks();

    for(int i = 0; i < MAX_PLAYERS; i++) {
        Player* p = &state->players[i];
        if(p->texture) {
            // --- ANIMATION UPDATE ---
            // Spara previous state statiskt per spelare
            if(p->animState != p->previousAnimState) {
                p->currentFrame = 0;
                p->previousAnimState = p->animState;
                p->animTimer = 0.0f;
            }

            int currentRow = 0;
            switch (p->animState) {
                case ANIM_IDLE: currentRow = 0; break;
                case ANIM_TAKE_DAM: currentRow = 1; break;
                case ANIM_ATT: currentRow = 2; break;
                case ANIM_DEATH: currentRow = 3; break;
                case ANIM_WALK: currentRow = 4; break;
            }

            float deltaTime = (now - p->lastTick) / 1000.0f;
            p->lastTick = now;

            float animSpeed = 0.1f;
            switch (p->animState) {
                case ANIM_IDLE: animSpeed = 0.2f; break;
                case ANIM_TAKE_DAM: animSpeed = 0.1f; break;
                case ANIM_ATT: animSpeed = 0.02f; break;
                case ANIM_DEATH: animSpeed = 0.15f; break;
                case ANIM_WALK: animSpeed = 0.05f; break;
            }

            p->animTimer += deltaTime;
            if(p->animTimer >= animSpeed) {
                p->animTimer = 0.0f;
                switch(p->animState) {
                    case ANIM_IDLE: p->currentFrame = (p->currentFrame+1)%6; break;
                    case ANIM_TAKE_DAM: p->currentFrame = (p->currentFrame+1)%5; break;
                    case ANIM_ATT: p->currentFrame = (p->currentFrame+1)%9; break;
                    case ANIM_DEATH: if(p->currentFrame<16) p->currentFrame++; break;
                    case ANIM_WALK: p->currentFrame = (p->currentFrame+1)%10; break;
                }
            }

            // --- COMPUTE SOURCE & DEST RECT ---
            SDL_FRect src = {
                p->currentFrame * PLAYER_SIZE,
                currentRow * PLAYER_SIZE,
                PLAYER_SIZE,
                PLAYER_SIZE
            };

            SDL_FRect dst = {
                state->camera.x + (state->players[0].pos.x - p->pos.x),
                state->camera.y + (state->players[0].pos.y - p->pos.y),
                PLAYER_SIZE * PLAYER_RENDER_SCALE,
                PLAYER_SIZE * PLAYER_RENDER_SCALE
            };

            SDL_FlipMode flip = p->facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            if(!SDL_RenderTextureRotated(state->renderer, p->texture, &src, &dst, 0.0, NULL, flip)) {
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


            !state->renderFlag;
            if(renderFrame(state)) return SDL_APP_FAILURE;
        }
    }
    return SDL_APP_CONTINUE;
}