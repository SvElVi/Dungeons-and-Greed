#include <SDL3/SDL.h>
#include "../inits.h"

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

typedef struct {
    Chunk chunks;
    Uint64 cord;
    struct XChunks* nextXChunk;
} XChunks;

typedef struct {
    XChunks* xChunks;
    Uint64 cord;
    struct YChunks* nextYChunk;
} YChunks;

struct world {
    YChunks quarter[4];
};

World createWorld();