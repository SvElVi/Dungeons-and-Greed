#ifndef WORLD_H
#define WORLD_H

#include "../inits.h"

World createWorld(int size, Uint64 seed, SDL_Renderer* renderer); //Create an array with memory to hold dungeon data

void destroyWorld(World w); //Delete the memory of the world from the heap

void createDungeon(World w, Uint8 nrOfRooms, AppState state, bool tp); //Add dungeon data to the world

void changeSeed(World w, Uint64 seed); //Change the world seed

bool renderDungeon(AppState state); //Render the vicinity of the local player

#endif
