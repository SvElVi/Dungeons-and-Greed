#ifndef PLAYER_H
#define PLAYER_H

#include "render.h"

bool collision(SDL_FRect a, SDL_FRect b);

bool willCollide(Player* player, Player players[MAX_PLAYERS], Enemy enemies[MAX_ENEMIES], float futureX, float futureY);

void movement(Player* player, Player players[MAX_PLAYERS], Enemy enemies[MAX_ENEMIES], int deltatime);

bool playerEnemyCollision(Player* player, Enemy enemies[MAX_ENEMIES], Uint32 deltatime);

void animatePlayers(Player players[MAX_PLAYERS], Uint8* counter, Uint16 framerate, bool* flag);

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer);

#endif