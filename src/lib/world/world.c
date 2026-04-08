#include <SDL3/SDL.h>
#include "../inits.h"

#define CHUNK_SIZE 16
#define ROOM_TYPES 1

typedef enum {
    BASIC
} ROOM_TYPE;

typedef enum {
    BLANK,
    FLOOR,
    WALL
} TILE_TYPE;

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

struct world {
    Chunk* chunks;
    Uint64 seed;
    int size;
    SDL_Texture* texture;
};

void cleanChunks(Chunk* chunks, int n) { //Set all tilevalues in chunks to 0
    for(int i = 0; i < n; i++) {
        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++) {
                chunks->tileType[y][x] = 0;
            }
        }
        chunks++;
    }
}

World createWorld(int size, Uint64 seed, SDL_Renderer* renderer) {
    World w = SDL_calloc(1, sizeof(struct world));
    w->chunks = SDL_calloc(size*size, sizeof(Chunk));
    w->size = size*size;

    SDL_Surface* wArt = SDL_LoadPNG("../img/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png");
    w->texture = SDL_CreateTextureFromSurface(renderer, wArt);
    SDL_DestroySurface(wArt);

    cleanChunks(w->chunks, w->size);

    return w;
}

void destroyWorld(World w) {
    SDL_free(w->chunks);
    if(w->texture) SDL_DestroyTexture(w->texture);
    SDL_free(w);
}

bool generateRoom(Chunk* c, int* wSize, Uint8* nrOfRooms) {
    Chunk* tempC = c;
    float hop = SDL_rand(4); //To make splits in same line more common
    int dir = (int)hop;

    switch(SDL_rand(ROOM_TYPES)) {
        case BASIC:
            for(int y = 0; y < CHUNK_SIZE; y++) {
                for(int x = 0; x < CHUNK_SIZE; x++) {
                    if(!x || x == (CHUNK_SIZE-1) || !y || y == (CHUNK_SIZE-1)) {
                        c->tileType[y][x] = 2; //Wall
                    } else {
                        c->tileType[y][x] = 1; //Floor
                    }
                }
            }
            break;
    }

    for(int i = 0; i < (SDL_rand(4)+1); i++) {
        switch(dir) {
            case WEST:
                tempC = c - 1;
                if(tempC->tileType[0][0] == 0) { //TODO: Add checks for boundries
                    SDL_Log("Test");
                    // generateRoom(temp, wSize, nrOfRooms);
                }
                break;
            case NORTH:
                tempC = c - (int)SDL_sqrt(*wSize);
                if(tempC->tileType[0][0] == 0) {
                    SDL_Log("Test");
                    // generateRoom(temp, wSize, nrOfRooms);
                }
                break;
            case EAST:
                tempC = c + 1;
                if(tempC->tileType[0][0] == 0) {
                    SDL_Log("Test");
                    // generateRoom(temp, wSize, nrOfRooms);
                }
                break;
            case SOUTH:
                tempC = c + (int)SDL_sqrt(*wSize);
                if(tempC->tileType[0][0] == 0) {
                    SDL_Log("Test");
                    // generateRoom(temp, wSize, nrOfRooms);
                }
                break;
        }
        SDL_Log("R: %d", dir);
        hop += 2.5;
        dir = (int)hop % 4;
    }
}

void generateDungeon(World w, Uint8* nrOfRooms) { //Room placements
    Chunk* temp;
    SDL_srand(w->seed);
    SDL_rand(4);

    SDL_Log("\n---DUNGEON TESTING---");

    switch(SDL_rand(4)) { //Starting room
        case WEST:
            SDL_Log("West");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2) * (int)SDL_sqrt(w->size); //Points to first chunk in middle row
            break;
        case NORTH:
            SDL_Log("North");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2); //Points to middle chunk in first row
            break;
        case EAST:
            SDL_Log("East");
            temp = w->chunks + (int)(SDL_sqrt(w->size)/2 + 1) * (int)SDL_sqrt(w->size)  -1; //Points to last chunk in middle row
            break;
        case SOUTH:
            SDL_Log("South");
            temp = w->chunks + w->size - (int)SDL_ceil(SDL_sqrt(w->size)/2); //Points to middle chunk in last row
            break;
    }

    SDL_Log("Dungeon start (1+): %d, TEST RANDOM: %d", temp - w->chunks + 1, SDL_rand(4));
    generateRoom(temp, &(w->size), nrOfRooms);

    SDL_Log("---------------------");
}

void polishDungeon(World w) { //Fix tileset in dungeon

}

void createDungeon(World w, Uint8 nrOfRooms) {
    generateDungeon(w, &nrOfRooms);
    polishDungeon(w);

}

void renderDungeon(AppState state) {
    int yLevel = 0;
    Chunk* tempC = state->world->chunks;
    const int rowSize = (int)SDL_sqrt(state->world->size);
    
    for(int i = 0; i < state->world->size; i++) {
        if(i % rowSize == 0 && i) { //Calc to change to next row, only counts when i is not 0
            yLevel++;
            // SDL_Log("%d", yLevel);
        }

        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++) {
                switch(tempC->tileType[y][x]) {
                    case FLOOR:
                        break;
                    case WALL:
                        break;
                }
            }
        }

        tempC++;
    }
}