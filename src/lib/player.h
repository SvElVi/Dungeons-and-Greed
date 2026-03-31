#include "render.h"

void movement(Player* player, int deltatime);

void updatePlayer(Player players[MAX_PLAYERS], int plNb, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer);