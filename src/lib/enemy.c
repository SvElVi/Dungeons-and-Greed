#include "enemy.h"
#include <math.h>

#define ENEMY_SPEED 0.15f * RENDER_SCALE

int nearestPlayer(Enemy* enemy, Player players[MAX_PLAYERS]) {
}

void enemyMovement(Enemy* enemy, Player players[MAX_PLAYERS], int deltatime) {
}

void animateEnemies(Enemy enemies[MAX_ENEMIES], Uint8* counter, Uint16 framerate) {
    ++*counter;
    if ((*counter) == (framerate / ANIMATION_TIME / 2) ||
        (*counter) == (framerate / ANIMATION_TIME)) {

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].state == ENEMY_DEAD) continue;

            bool moving = (enemies[i].state == ENEMY_CHASE);
            AniState row;

            if (moving) {
                switch (enemies[i].facing) {
                    case WEST:  row = WALK_HORIZONTAL; enemies[i].flip = SDL_FLIP_HORIZONTAL; break;
                    case EAST:  row = WALK_HORIZONTAL; enemies[i].flip = SDL_FLIP_NONE;       break;
                    case NORTH: row = WALK_NORTH;      enemies[i].flip = SDL_FLIP_NONE;       break;
                    case SOUTH: row = WALK_SOUTH;      enemies[i].flip = SDL_FLIP_NONE;       break;
                }
            } else {
                if ((*counter) != (framerate / ANIMATION_TIME)) continue;
                switch (enemies[i].facing) {
                    case WEST:  row = IDLE_HORIZONTAL; enemies[i].flip = SDL_FLIP_HORIZONTAL; break;
                    case EAST:  row = IDLE_HORIZONTAL; enemies[i].flip = SDL_FLIP_NONE;       break;
                    case NORTH: row = IDLE_NORTH;      enemies[i].flip = SDL_FLIP_NONE;       break;
                    case SOUTH: row = IDLE_SOUTH;      enemies[i].flip = SDL_FLIP_NONE;       break;
                }
            }

            enemies[i].aniBox.y = row * ENEMY_SIZE;
            enemies[i].aniBox.x += ENEMY_SIZE;
            enemies[i].aniBox.x = (float)((int)enemies[i].aniBox.x % (ENEMY_SIZE * 6));
        }
    }

    if ((*counter) == (framerate / ANIMATION_TIME))
        (*counter) = 0;
}

static void updateEnemyClass(Enemy* enemy, SDL_Renderer* renderer) {
    SDL_Surface* eArt;
    switch (enemy->type) {
        case ENEMY_SKELETON:    eArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/skeleton1/monsters_idle/v1/skeleton_v1_1.png");    break;
        case ENEMY_VAMPIRE:   eArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/vampire/monsters_idle/v1/vampire_v1_1.png");   break;
    }
    enemy->texture = SDL_CreateTextureFromSurface(renderer, eArt);
    SDL_DestroySurface(eArt);
}

void updateEnemy(Enemy* enemy, Vector2D pos, Enemy_Type type, Stats stats, SDL_Renderer* renderer) {
    enemy->aniBox.w   = ENEMY_SIZE;
    enemy->aniBox.h   = ENEMY_SIZE;
    enemy->hitBox.w   = 9 * RENDER_SCALE;
    enemy->hitBox.h   = 2 * RENDER_SCALE;
    enemy->aniBox.x   = 0;
    enemy->aniBox.y   = 0;
    enemy->pos        = pos;
    enemy->hitBox.x   = pos.x;
    enemy->hitBox.y   = pos.y;
    enemy->type       = type;
    enemy->state      = ENEMY_IDLE;
    enemy->facing     = SOUTH;
    enemy->stats      = stats;
    enemy->moveX      = 0;
    enemy->moveY      = 0;
    updateEnemyClass(enemy, renderer);
}