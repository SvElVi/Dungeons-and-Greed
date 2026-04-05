#include <SDL3/SDL.h>
#include "../inits.h"

#define CHUNK_SIZE 16
#define ROOM_TYPES 10

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

void polishDungeon(World w) { //Fix tileset in dungeon

}

void generateDungeon(World w, Uint64 seed, int nrOfRooms) {
    Sint32 randList;

    SDL_srand(seed);
    randList = SDL_rand(ROOM_TYPES) + 1;


    SDL_Log("Failed generating some rooms.");
}

void createDungeon(World w, Uint64 seed, int nrOfRooms) {
    generateDungeon(w, seed, nrOfRooms);
    polishDungeon(w);

}

void renderDungeon(AppState state) {
    for(int i = 0; i < state->world->size; i++) {
        if(i % (int)SDL_sqrt(state->world->size) == 0) {
            SDL_Log("%d", i);
        }
    }
}