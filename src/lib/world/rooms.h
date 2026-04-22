#include <SDL3/SDL.h>

#define CHUNK_SIZE 48
#define ROOM_TYPES 1
#define TILE_SIZE 16

typedef enum {
    TEST
} ROOM_TYPE;

typedef enum {
    BLANK,
    FLOOR,
    WALL
} TILE_TYPE;

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

Uint64 CircleRoom[48] = {
    0x0B180B,
    0xFF,
    0x00
};
