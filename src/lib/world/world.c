#include <SDL3/SDL.h>
#include "world.h"

#define CHUNK_SIZE 16

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

typedef struct {
    Chunk chunk;
    int cord;
    struct XChunks* nextXChunk;
} XChunks;

typedef struct {
    XChunks* xChunks;
    int cord;
    struct YChunks* nextYChunk;
} YChunks;

struct world {
    YChunks* yChunks;
};

World createWorld() {
    World w = SDL_calloc(1, sizeof(struct world));
    return w;
}

void destroyWorld(World w) {
    SDL_free(w);
}

void createChunk(World w) {
    if(w->yChunks = NULL) {
        SDL_Log("Test");
    }
}