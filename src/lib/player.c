#include "inits.h"
#define SPEED 0.5

void movement(Player* player, int deltatime) {
    if(player->flags.moveX != 0) {
        player->pos.x -= player->flags.moveX * deltatime * SPEED;
        player->facing = player->flags.moveX + 1;
    }
    if(player->flags.moveY != 0) {
        player->pos.y -= player->flags.moveY * deltatime * SPEED;
        player->facing = player->flags.moveY + 2;
    }
}

void playerAnimate(Player* player, Uint8* counter) {
    *counter++;
    if(!(*counter < ANIMATION_TIME)) {
        *counter = 0;
        switch(player->facing) {
            case WEST:
                if(player->flags.moveX != 0) {
                    player->aniBox.x = WALK_HORIZONTAL * PLAYER_SIZE;
                } else {
                    player->aniBox.x = IDLE_HORIZONTAL * PLAYER_SIZE;
                }
                break;
            case NORTH:
                if(player->flags.moveY != 0) {
                    player->aniBox.y = WALK_NORTH * PLAYER_SIZE;
                } else {
                    player->aniBox.y = IDLE_NORTH * PLAYER_SIZE;
                }
                break;
            case EAST:
                if(player->flags.moveX != 0) {
                    player->aniBox.x = WALK_HORIZONTAL * PLAYER_SIZE;
                } else {
                    player->aniBox.x = IDLE_HORIZONTAL * PLAYER_SIZE;
                }
                break;
            case SOUTH:
                if(player->flags.moveY != 0) {
                    player->aniBox.y = WALK_SOUTH * PLAYER_SIZE;
                } else {
                    player->aniBox.y = IDLE_SOUTH * PLAYER_SIZE;
                }
                break;
    }
    }
}

void updateClass(Player* player, SDL_Renderer* renderer) {
    SDL_Surface* pArt;
    switch (player->class) { //Paths below should be change in the future
        case CLASS_NONE:
            pArt = SDL_LoadPNG("./img/Custom/Player.png");
            break;
        case CLASS_MAGE:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_PRIEST:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_HUNTER:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_SWORDMASTER:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_KNIGHT:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
    }
    player->texture = SDL_CreateTextureFromSurface(renderer, pArt);
    SDL_DestroySurface(pArt);
}

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer) {
    player->aniBox.w = PLAYER_SIZE * PLAYER_RENDER_SCALE;
    player->aniBox.h = PLAYER_SIZE * PLAYER_RENDER_SCALE;
    player->aniBox.x = 0;
    player->aniBox.y = 0;
    player->pos = pos;
    player->class = class;
    updateClass(player, renderer);
    player->stats = stats;
    SDL_Log("Pos: %d Class: %d Stats: %d", pos, class, stats);
}