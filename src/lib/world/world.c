#include <SDL3/SDL.h>
#include "world.h"

#define CHUNK_SIZE 16

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

struct world {
    Chunk* chunks;
    int size;
};

World createWorld(int size) {
    World w = SDL_calloc(1, sizeof(struct world));
    w->chunks = SDL_calloc(size*size, sizeof(Chunk));
    return w;
}

void destroyWorld(World w) {
    SDL_free(w->chunks);
    SDL_free(w);
}