#include "inits.h"
#define SPEED 0.5

void movement(Player* player, int deltatime) {
    if(player->flags.moveX != 0) player->pos.x += player->flags.moveX * deltatime * SPEED;
    if(player->flags.moveY != 0) player->pos.y += player->flags.moveY * deltatime * SPEED;
    // SDL_Log("PosX: %d PosY: %d", player->pos.x, player->pos.y);
}