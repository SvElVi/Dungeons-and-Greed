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
 //&& (player->flags.moveX || player->flags.moveY)
 // || 
void animatePlayers(Player players[MAX_PLAYERS], Uint8* counter, bool* flag) {
    ++*counter;
    if(*counter >= ANIMATION_TIME/2) {
        for(int i = 0; i < MAX_PLAYERS; i++) {
            if(players[i].flags.moveX || players[i].flags.moveY || (*flag && *counter >= ANIMATION_TIME)) { //Render faster if moving
                SDL_Log("Test: %d", *flag);
                switch(players[i].facing) {
                    case WEST:
                        if(players[i].flags.moveX != 0) {
                            players[i].aniBox.y = WALK_HORIZONTAL * PLAYER_SIZE;
                        } else {
                            players[i].aniBox.y = IDLE_HORIZONTAL * PLAYER_SIZE;
                        }
                        break;
                    case NORTH:
                        if(players[i].flags.moveY != 0) {
                            players[i].aniBox.y = WALK_NORTH * PLAYER_SIZE;
                        } else {
                            players[i].aniBox.y = IDLE_NORTH * PLAYER_SIZE;
                        }
                        break;
                    case EAST:
                        if(players[i].flags.moveX != 0) {
                            players[i].aniBox.y = WALK_HORIZONTAL * PLAYER_SIZE;
                        } else {
                            players[i].aniBox.y = IDLE_HORIZONTAL * PLAYER_SIZE;
                        }
                        break;
                    case SOUTH:
                        if(players[i].flags.moveY != 0) {
                            players[i].aniBox.y = WALK_SOUTH * PLAYER_SIZE;
                        } else {
                            players[i].aniBox.y = IDLE_SOUTH * PLAYER_SIZE;
                        }
                        break;
                }

                players[i].aniBox.x += PLAYER_SIZE;
                players[i].aniBox.x = (float)((int)players[i].aniBox.x % (PLAYER_SIZE*6));

                if(i == MAX_PLAYERS-1) {
                    *counter = 0;
                }
            }
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
    player->aniBox.w = PLAYER_SIZE;
    player->aniBox.h = PLAYER_SIZE;
    player->aniBox.x = 0;
    player->aniBox.y = 0;
    player->pos = pos;
    player->class = class;
    updateClass(player, renderer);
    player->stats = stats;
    SDL_Log("Pos: %d Class: %d Stats: %d", pos, class, stats);
}