#include "enemy.h"
#include <math.h>

#define ENEMY_SPEED 0.15f * RENDER_SCALE
#define ENEMY_FRAMES 4

int nearestPlayer(Enemy *enemy, Player players[MAX_PLAYERS])
{
}

void enemyMovement(Enemy *enemy, Player players[MAX_PLAYERS], int deltatime)
{
}

void animateEnemies(Enemy enemies[MAX_ENEMIES], Uint8 *counter, Uint16 framerate, bool *flag)
{
    ++*counter;

    if ((*counter) == (framerate / ANIMATION_TIME / 2) || (*counter) == (framerate / (ANIMATION_TIME)))
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].state == ENEMY_DEAD)
                continue;

            bool moving = (enemies[i].state == ENEMY_CHASE);
            int row = 0;

            if (moving)
            {
                switch (enemies[i].facing)
                {
                case WEST:
                    enemies[i].flip = SDL_FLIP_HORIZONTAL;
                    break;
                case EAST:
                    row = 2;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                case NORTH:
                    row = 1;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                case SOUTH:
                    row = 0;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                }
            }
            else
            {
                if ((*counter) != (framerate / ANIMATION_TIME))
                    continue;

                switch (enemies[i].facing)
                {
                case WEST:
                    row = 2;
                    enemies[i].flip = SDL_FLIP_HORIZONTAL;
                    break;
                case EAST:
                    row = 2;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                case NORTH:
                    row = 1;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                case SOUTH:
                    row = 0;
                    enemies[i].flip = SDL_FLIP_NONE;
                    break;
                }
            }

            enemies[i].aniBox.y = row * ENEMY_SIZE;
            enemies[i].aniBox.x += ENEMY_SIZE;

            enemies[i].aniBox.x = (float)((int)enemies[i].aniBox.x % (ENEMY_SIZE * 6));
        }
    }

    if ((*counter) == (framerate / ANIMATION_TIME))
    {
        (*counter) = 0;
    }
}

static void updateEnemyClass(Enemy *enemy, SDL_Renderer *renderer)
{
    SDL_Surface *eArt;
    switch (enemy->type)
    {
    case ENEMY_SKELETON:
        eArt = SDL_LoadPNG("img/Custom/Skeleton.png");
        break;
    }

    enemy->texture = SDL_CreateTextureFromSurface(renderer, eArt);
    SDL_DestroySurface(eArt);
}

void updateEnemy(Enemy *enemy, Vector2D pos, Enemy_Type type, Stats stats, SDL_Renderer *renderer)
{
    enemy->aniBox.w = ENEMY_SIZE;
    enemy->aniBox.h = ENEMY_SIZE;
    enemy->hitBox.w = 20 * RENDER_SCALE;
    enemy->hitBox.h = 10 * RENDER_SCALE;
    enemy->aniBox.x = 0;
    enemy->aniBox.y = 0;
    enemy->pos = pos;
    enemy->hitBox.x = enemy->pos.x;
    enemy->hitBox.y = enemy->pos.y;
    enemy->type = type;
    enemy->state = ENEMY_IDLE;
    enemy->facing = SOUTH;
    enemy->stats = stats;
    enemy->moveX = pos.x;
    enemy->moveY = pos.y;
    updateEnemyClass(enemy, renderer);
}