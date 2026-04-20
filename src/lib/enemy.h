#ifndef ENEMY_H
#define ENEMY_H

#include "render.h"

int nearestPlayer(Enemy* enemy, Player players[MAX_PLAYERS]);

void enemyMovement(Enemy* enemy, Player players[MAX_PLAYERS], int deltatime);

void animateEnemies(Enemy enemies[MAX_ENEMIES], Uint8* counter, Uint16 framerate, bool* flag);

static void updateEnemyClass(Enemy* enemy, SDL_Renderer* renderer);

void updateEnemy(Enemy* enemy, Vector2D pos, Enemy_Type type, Stats stats, SDL_Renderer* renderer);

#endif