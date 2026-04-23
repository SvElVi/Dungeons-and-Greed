#include <SDL3/SDL.h>

#define CHUNK_SIZE 48
#define ROOM_TYPES 2
#define TILE_SIZE 16

typedef enum {
    TEST,
    ROOM_CIRCLE
} ROOM_TYPE;

typedef enum {
    BLANK,
    FLOOR,
    WALL
} TILE_TYPE;

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

const Uint64 CircleRoom[24] = {
    0x1221060F,
    0xF0,
    0x1224030F,
    0x15240F,

    0x18230D,
    0x1A230B,
    0x1C220A,
    0x1D2308,
    
    0x00
};
