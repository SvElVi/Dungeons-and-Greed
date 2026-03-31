#include "inits.h"
#define SPEED 0.5

void movement(Player* player, int deltatime) {
    if(player->flags.moveX != 0) player->renderBox.x += player->flags.moveX * deltatime * SPEED;
    if(player->flags.moveY != 0) player->renderBox.y += player->flags.moveY * deltatime * SPEED;
}