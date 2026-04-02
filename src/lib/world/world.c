#include <SDL3/SDL.h>
#include "world.h"

#define CHUNK_SIZE 16

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

typedef struct XChunks {
    Chunk chunk;
    int cord;
    struct XChunks* nextXChunk;
} XChunks;

typedef struct YChunks {
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
    if(w) {
        if(w->yChunks) {
            YChunks* yop = w->yChunks;
            YChunks* yp;
            do {
                if(w->yChunks->xChunks) {
                    XChunks* xop = w->yChunks->xChunks;
                    XChunks* xp;
                    do {
                        xp = xop->nextXChunk;
                        SDL_free(xop);
                        xop = xp;
                        SDL_Log("Found Mem X");
                    } while (xp != NULL);
                }
                yp = yop->nextYChunk;
                SDL_free(yop);
                yop = yp;
                SDL_Log("Found Mem Y");
            } while (yp != NULL);
        }
        SDL_free(w);
    }
}

void createChunk(World w) {
    if(w->yChunks == NULL) {
        w->yChunks = SDL_calloc(1, sizeof(YChunks));
    }
}