#include "render.h"

void movement(Player* player, int deltatime);

void playerAnimate(Player* player, int framerate);

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer);