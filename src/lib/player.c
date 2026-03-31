#include "inits.h"
#define SPEED 0.5

void movement(Player* player, int deltatime) {
    if(player->flags.moveX != 0) player->pos.x += player->flags.moveX * deltatime * SPEED;
    if(player->flags.moveY != 0) player->pos.y += player->flags.moveY * deltatime * SPEED;
    // SDL_Log("PosX: %d PosY: %d", player->pos.x, player->pos.y);
}

void updateClass(Player players[MAX_PLAYERS], int plNb, SDL_Renderer* renderer) {
    SDL_Surface* pArt;
    switch (players[plNb].class) { //Paths below should be change in the future
        case CLASS_NONE:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
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
    players[plNb].texture = SDL_CreateTextureFromSurface(renderer, pArt);
    SDL_DestroySurface(pArt);
}

void updatePlayer(Player players[MAX_PLAYERS], int plNb, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer) {
    if(plNb >= MAX_PLAYERS) {
        plNb = 0;
        SDL_Log("WARNING, OUT OF BOUND PLAYER VALUE. VALUE SET TO ZERO!");
    }
    players[plNb].pos = pos;
    players[plNb].class = class;
    updateClass(players, plNb, renderer);
    players[plNb].stats = stats;
    SDL_Log("Pos: %d Class: %d Stats: %d", pos, class, stats);
}