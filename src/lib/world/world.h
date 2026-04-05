#include "../inits.h"

World createWorld(int size);

void destroyWorld(World w);

void createDungeon(World w, Uint64 seed, int nrOfRooms);

void renderDungeon(AppState state);