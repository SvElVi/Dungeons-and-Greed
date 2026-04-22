#include "hud.h"

bool drawPlayerNameBelow(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect)
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

bool drawHpBarAbove(SDL_Renderer *renderer, const Stats *stats, const SDL_FRect *spriteRect)
{
    
    float barHeight = 4.0f * RENDER_SCALE;
    float barWidth = spriteRect->w * 0.5;

    float hpRatio = 0.0f;
    if (stats->maxHealth > 0) {
        hpRatio = (float)stats->health / (float)stats->maxHealth;
    }

    if (hpRatio < 0.0f) 
        hpRatio = 0.0f;
    else if (hpRatio > 1.0f) 
        hpRatio = 1.0f;

    SDL_FRect backGroundRect = {
        spriteRect->x + (spriteRect->w - barWidth) * 0.5f,
        spriteRect->y + 4*barHeight,    // adding y axel decrease the distance gap
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
