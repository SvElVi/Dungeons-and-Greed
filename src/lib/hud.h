#ifndef HUD_H
#define HUD_H

#include "events.h"
#include "player.h"
#include "enemy.h"
#include <string.h>
#include "render.h"

bool drawHpBarAbove(SDL_Renderer *renderer, const Stats *stats, const SDL_FRect *spriteRect);
bool drawPlayerNameBelow(SDL_Renderer *renderer, const Player *player, const SDL_FRect *spriteRect);

#endif