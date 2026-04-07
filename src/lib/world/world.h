#include "../inits.h"

World createWorld(int size); //Create an array with memory to hold dungeon data

void destroyWorld(World w); //Delete the memory of the world from the heap

void createDungeon(World w, Uint64 seed, Uint8 nrOfRooms); //Add dungeon data to the world

void renderDungeon(AppState state); //Render the vicinity of the local player