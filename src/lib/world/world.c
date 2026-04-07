#include <SDL3/SDL.h>
#include "../inits.h"

#define CHUNK_SIZE 16
#define ROOM_TYPES 1

typedef enum {
    BASIC
} ROOM_TYPE;

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

struct world {
    Chunk* chunks;
    Uint64 seed;
    int size;
};

World createWorld(int size) {
    World w = SDL_calloc(1, sizeof(struct world));
    w->chunks = SDL_calloc(size*size, sizeof(Chunk));
    w->size = size*size;
    return w;
}

void destroyWorld(World w) {
    SDL_free(w->chunks);
    SDL_free(w);
}

void generateRoom(Chunk* c, int* wSize, Uint8* nrOfRooms) {
    Chunk* temp;

    switch(SDL_rand(ROOM_TYPES)) {
        case BASIC:
            break;
    }

    direction dir = SDL_rand(3);
    switch(dir) {
        case WEST:
            temp = c;
            if(--temp) {
                // generateRoom(temp, wSize, nrOfRooms);
            }
        case NORTH:
            temp = c;
            temp -= (int)SDL_sqrt(*wSize);
            if(temp) {
                // generateRoom(temp, wSize, nrOfRooms);
            }
        case EAST:
            temp = c;
            if(++temp) {
                // generateRoom(temp, wSize, nrOfRooms);
            }
            break;
    }
}

void generateDungeon(World w, Uint64 seed, Uint8* nrOfRooms) { //Room placements
    Chunk* temp;
    SDL_srand(seed);
    SDL_rand(4);

    SDL_Log("\n---DUNGEON TESTING---");

    switch(SDL_rand(4)) { //Starting room
        case WEST:
            SDL_Log("West");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2) * (int)SDL_sqrt(w->size); //Points to first chunk in middle row
            break;
        case NORTH:
            SDL_Log("North");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2); //Points to middle chunk in first row
            break;
        case EAST:
            SDL_Log("East");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2 + 1) * (int)SDL_sqrt(w->size)  -1; //Points to last chunk in middle row
            break;
        case SOUTH:
            SDL_Log("South");
            temp = w->chunks + w->size - (int)SDL_ceil(SDL_sqrt(w->size)/2); //Points to middle chunk in last row
            break;
    }

    SDL_Log("Dungeon start (1+): %d, TEST RANDOM: %d", temp - w->chunks + 1, SDL_rand(4));
    generateRoom(temp, &(w->size), nrOfRooms);

    SDL_Log("---------------------");
}

void polishDungeon(World w) { //Fix tileset in dungeon

}

void createDungeon(World w, Uint64 seed, Uint8 nrOfRooms) {
    generateDungeon(w, seed, &nrOfRooms);
    polishDungeon(w);

}

void renderDungeon(AppState state) {
    int yLevel = 0;
    const int rowSize = (int)SDL_sqrt(state->world->size);
    
    for(int i = 0; i < state->world->size; i++) {
        if(i % rowSize == 0 && i) { //Calc to change to next row, only counts when i is not 0
            yLevel++;
            // SDL_Log("%d", yLevel);
        }
    }
}