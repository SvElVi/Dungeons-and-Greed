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

bool generateRoom(Chunk* c, int* wSize) {
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
                generateRoom(temp, wSize);
            }
        case NORTH:
            temp = c;
            temp -= (int)SDL_sqrt(*wSize);
            if(temp) {
                generateRoom(temp, wSize);
            }
        case EAST:
            temp = c;
            if(++temp) {
                generateRoom(temp, wSize);
            }
            break;
    }
}

void generateDungeon(World w, Uint64 seed, Uint8 nrOfRooms) { //Room placements
    SDL_srand(seed);



    // SDL_Log("Failed generating some rooms.");
}

void polishDungeon(World w) { //Fix tileset in dungeon

}

void createDungeon(World w, Uint64 seed, int nrOfRooms) {
    generateDungeon(w, seed, nrOfRooms);
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