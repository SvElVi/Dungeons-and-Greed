#include <SDL3/SDL.h>
#include "../inits.h"

#define CHUNK_SIZE 16
#define ROOM_TYPES 1
#define TILE_SIZE 16
#define TILE_RENDER_SCALE 2

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

    SDL_Surface* wArt = SDL_LoadPNG("././img/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png");
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

bool generateRoom(Chunk* org, Chunk* c, int* wSize, Uint8* nrOfRooms) { //org for origin pointer, c for relative, wSize for boundries, nrOfRooms for room limit
    Chunk* tempC = c;
    float hop = SDL_rand(4); //To make splits in same line more common
    int dir = (int)hop, pDif;
    const int rowSize = (int)SDL_sqrt(*wSize);

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

    for(int i = 0; i < (SDL_rand(4)+1) && (*nrOfRooms) > 0; i++) {
        switch(dir) {
            case WEST:
                tempC = c - 1;
                break;
            case NORTH:
                tempC = c - rowSize;
                break;
            case EAST:
                tempC = c + 1;
                break;
            case SOUTH:
                tempC = c + rowSize;
                break;
        }
        pDif = tempC - org;
        
        if(dir == NORTH || dir == SOUTH) {
            if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0) { //Check for if withing boundries and available space
                SDL_Log("Valid v");
                (*nrOfRooms)--;
                generateRoom(org, tempC, wSize, nrOfRooms);
            }
        } else if(dir = WEST) {
            if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0 && !((pDif % rowSize) == rowSize-1)) { //Check extra for end of row
                SDL_Log("Valid v");
                (*nrOfRooms)--;
                generateRoom(org, tempC, wSize, nrOfRooms);
            }
        } else if(dir = EAST) {
            if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0 && !((pDif % rowSize) == 0)) { //Check extra for end of row
                SDL_Log("Valid v");
                (*nrOfRooms)--;
                generateRoom(org, tempC, wSize, nrOfRooms);
            }
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
    generateRoom(w->chunks, temp, &(w->size), nrOfRooms);

    SDL_Log("---------------------");
}

void polishDungeon(World w) { //Fix tileset in dungeon

}

void createDungeon(World w, Uint8 nrOfRooms) {
    generateDungeon(w, &nrOfRooms);
    polishDungeon(w);

}

bool renderDungeon(AppState state) {
    int yLevel = 0;
    Chunk* tempC = state->world->chunks;
    const int rowSize = (int)SDL_sqrt(state->world->size);
    SDL_FRect srcRect = {0,0,TILE_SIZE,TILE_SIZE}, dstRect = {0,0,TILE_SIZE*TILE_RENDER_SCALE,TILE_SIZE*TILE_RENDER_SCALE};
    
    for(int i = 0; i < state->world->size; i++) {
        if(i % rowSize == 0 && i) { //Calc to change to next row, only counts when i is not 0
            yLevel++;
        }

        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++) {
                dstRect.x = state->camera.x + state->players[0].pos.x + (x+CHUNK_SIZE*((tempC - state->world->chunks) % rowSize))*TILE_SIZE*TILE_RENDER_SCALE - rowSize*CHUNK_SIZE*TILE_SIZE*TILE_RENDER_SCALE/2;
                dstRect.y = state->camera.y + state->players[0].pos.y + (y+CHUNK_SIZE*((int)(tempC - state->world->chunks) / rowSize))*TILE_SIZE*TILE_RENDER_SCALE - rowSize*CHUNK_SIZE*TILE_SIZE*TILE_RENDER_SCALE/2;
                
                switch(tempC->tileType[y][x]) {
                    case FLOOR:
                        srcRect.x = TILE_SIZE*2;
                        srcRect.y = TILE_SIZE*2;
                        if(!SDL_RenderTexture(state->renderer, state->world->texture, &srcRect, &dstRect)) {
                            SDL_Log("TILE RENDER ERROR: %d : %s", tempC->tileType[y][x], SDL_GetError());
                            return 0;
                        }
                        break;
                    case WALL:
                        srcRect.x = TILE_SIZE*1;
                        srcRect.y = TILE_SIZE*0;
                        if(!SDL_RenderTexture(state->renderer, state->world->texture, &srcRect, &dstRect)) {
                            SDL_Log("TILE RENDER ERROR: %d : %s", tempC->tileType[y][x], SDL_GetError());
                            return 0;
                        }
                        break;
                }
            }
        }

        tempC++;
    }

    return 1;
}