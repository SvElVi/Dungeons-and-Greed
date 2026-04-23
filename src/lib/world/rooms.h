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
    0x090F,
    0xF0,
    0x26030F,
    0x15240F,

    0x18230D,
    0x1A230B,
    0x1C220A,
    0x1D2308,

    0x1F2207,
    0x111F2107,
    0x111F2206,
    0x121F2205,

    0x131F2105,
    0x131F2204,
    0x21131F2104,
    0x21131F2203,

    0x151F2103,
    0x15221D2103,
    0x15221D2202,
    0x161F2102,

    0xF0,
    0xF0,
    0xF0,
    0x18221B2102
};
