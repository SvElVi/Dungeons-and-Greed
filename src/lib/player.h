#include "render.h"

void movement(Player* player, int deltatime);

void playerAnimate(Player* player, Uint8* counter);

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer);