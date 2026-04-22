#include <SDL3/SDL.h>

#define CHUNK_SIZE 48
#define ROOM_TYPES 1
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
    0xFF,
    0x1223040F,
    0x1523010F,
    0x18220E,
    0x1A220C,
    0x1C210B,
    0x00
};
