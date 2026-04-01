#include "render.h"

void movement(Player* player, int deltatime);

void animatePlayers(Player players[MAX_PLAYERS], Uint8* counter, int framerate, bool* flag);

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer);