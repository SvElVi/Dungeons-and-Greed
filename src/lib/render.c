#include "player.h"
#include <string.h>

static bool drawHpBarAbove(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect);
static bool drawPlayerNameBelow(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect);

int renderFrame(AppState state) {

    if(state->gameState == GAME_MENY){
        menu_screen(&state->mainMenu, state);
    }
    else if(state->gameState == GAME_PAUSE){
        pause_screen(state, "Press SPACE to continue");   
    }
    else if(state->gameState == GAME_HOST){
        host_screen(state);
    }
    else if(state->gameState == GAME_JOIN){
        join_screen(state);
    }
    else if(state->gameState == GAME_TCP_INIT){
        pause_screen(state, "Connecting...");
    }
    else if(state->gameState == GAME_TCP_HANDSHAKE){
        pause_screen(state, "Handshake...");
    }
    else if(state->gameState == GAME_TCP_VERIFYING_HANDSHAKE){
        pause_screen(state, "Veryfying handshake...");
    }
    
    else if (state->gameState == GAME_PLAYING) {
        renderGamePlay(state);
    }
    SDL_RenderPresent(state->renderer);
    state->computedEvent = false;
    
    return SDL_APP_CONTINUE;
}

int render(AppState state) { //current but should be changed to call back style, also with vsync and variable refreshrate
    Uint64 currentTime = SDL_GetTicks();
    if(currentTime >= state->lastTime + SDL_round(1000/state->framerate)){ //renderflag unused
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

void menu_screen(Menu *menu, AppState state){
    int w, h;
    float titleX, titleY, menuY;
    float x, y;
    const float scale = 4.0f;
    const char *title = "GREEDY DELVERS";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);

    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    titleX = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) / 2;
    titleY = (h / scale) * 0.1f;    
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, titleX, titleY, title);
    menuY= (h / scale) *0.4f;
    for(int i = 0; i < menu->count; i++)
    {
        x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE* SDL_strlen(menu->menuOptions[i]))/2;
        y = menuY + i *20;

        if(i == menu->selected)
        {
            SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        }
        SDL_RenderDebugText(state->renderer, x, y, menu->menuOptions[i]);
    }
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void join_screen(AppState state){
    int w, h;
    float  x, y;
    const float scale = 4.0f;
    const char *title = "ENTER IP HOST to join:";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);
    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    x = ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) /2;
    y = (h/ scale) /2;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, x, y, title);

    y += 40;
    x= ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE *SDL_strlen(state->hostIP)) /2;

    SDL_RenderDebugText(state->renderer, x, y, state->hostIP);
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void host_screen(AppState state){
    int w, h;
    float  x, y;
    const float scale = 4.0f;
    const char *title = "ENTER IP HOST";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);
    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    x = ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) /2;
    y = (h/ scale) /2;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, x, y, title);

    y += 40;
    x= ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE *SDL_strlen(state->hostIP)) /2;

    SDL_RenderDebugText(state->renderer, x, y, state->hostIP);
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void pause_screen(AppState state, const char *inputMes){
        const char *message = inputMes;
        const char *title = "GREEDY DELVERS";
        int w = 0, h = 0;
        float titleX, titleY;
        float x, y;
        const float scale = 4.0f;

        /* Center the message and scale it up */
        SDL_GetRenderOutputSize(state->renderer, &w, &h);
        SDL_SetRenderScale(state->renderer, scale, scale);
        x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
        y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;
        titleX = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
        titleY = (h / scale) * 0.05f;

        /* Draw the message */
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->renderer);
        SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(state->renderer, x, y, message);
        SDL_RenderDebugText(state->renderer, titleX, titleY, title);
        SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
    } 

int renderGamePlay(AppState state){
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

                if (!drawHpBarAbove(state->renderer, &state->players[renderOrder[i].x], &temp)) {
                    return SDL_APP_FAILURE;
                }

                if (!drawPlayerNameBelow(state->renderer, &state->players[renderOrder[i].x], &temp)) {
                    return SDL_APP_FAILURE;
                }
                
            }
    }
}

static bool drawHpBarAbove(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect)
{
    
    float barHeight = 4.0f * RENDER_SCALE;
    float barWidth = spriteRect->w;

    float hpRatio = 0.0f;
    if (player->stats.maxHealth > 0) {
        hpRatio = (float)player->stats.health / (float)player->stats.maxHealth;
    }

    if (hpRatio < 0.0f) 
        hpRatio = 0.0f;
    else if (hpRatio > 1.0f) 
        hpRatio = 1.0f;

    SDL_FRect backGroundRect = {
        spriteRect->x,
        spriteRect->y,
        barWidth,
        barHeight
    };

    SDL_FRect fillRect = {
        backGroundRect.x + 1.0f,
        backGroundRect.y + 1.0f,
        (backGroundRect.w - 2.0f) * hpRatio,
        backGroundRect.h - 2.0f
    };

    // draw background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    if (!SDL_RenderFillRect(renderer, &backGroundRect)) {
        SDL_Log("FAILED DRAWING HP BAR BACKGROUND: %s", SDL_GetError());
        return false;
    }

    // draw red hp bar fill
    SDL_SetRenderDrawColor(renderer, 200, 40, 40, 255);
    if (!SDL_RenderFillRect(renderer, &fillRect)) {
        SDL_Log("FAILED DRAWING HP BAR FILL: %s", SDL_GetError());
        return false;
    }

    // draw white border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (!SDL_RenderRect(renderer, &backGroundRect)) {
        SDL_Log("FAILED DRAWING HP BAR BORDER: %s", SDL_GetError());
        return false;
    }

    return true;
}

static bool drawPlayerNameBelow(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect)
{
    float textWidth = (float)(SDL_strlen(player->name) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE);

    float textX = spriteRect->x + (spriteRect->w - textWidth) * 0.5f;   // centrerad x axel
    float textY = spriteRect->y + spriteRect->h;    // right below character y-axel

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (!SDL_RenderDebugText(renderer, textX, textY, player->name)) {
        SDL_Log("FAILED DRAWING PLAYER NAME: %s", SDL_GetError());
        return false;
    }

    return true;
}
