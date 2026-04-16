#include <SDL3/SDL.h>
#include "../inits.h"

#define CHUNK_SIZE 32
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

struct world {
    Chunk* chunks;
    Uint64 seed;
    int size;
    SDL_Texture* texture;
};

void cleanChunks(Chunk* chunks, int size) { //Set all tilevalues in chunks to 0
    for(int i = 0; i < size; i++) {
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

    SDL_Surface* wArt = SDL_LoadPNG("././img/Custom/Dungeon_Tileset.png");
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

void changeSeed(World w, Uint64 seed) {
    w->seed = seed;
}

bool generateRoom(Chunk* org, Chunk* c, int* wSize, Uint8* nrOfRooms, Uint8 fDir) { //org for origin pointer, c for relative, wSize for boundries, nrOfRooms for room limit, fDir for the direction from previus generation
    Chunk* tempC = c;
    float hop = SDL_rand(4); //To make splits in same line more common
    int dir = (int)hop, pDif;
    const int rowSize = (int)SDL_sqrt(*wSize);
    bool genDir[4] = {0}; //For generating exits, default all false

    c->tileType[0][0] = 99; 

    if(fDir < 4) {
        fDir = (fDir + 2) % 4;
        genDir[fDir] = true;
    }

    for(int i = 0; i < (SDL_rand(3)+2) && (*nrOfRooms) > 0; i++) {
        switch(dir) {
            case WEST:
                tempC = c - 1;
                pDif = tempC - org;
                if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0 && !((pDif % rowSize) == rowSize-1)) { //Check extra for end of row
                    genDir[WEST] = true;
                    (*nrOfRooms)--;
                    generateRoom(org, tempC, wSize, nrOfRooms, dir);
                }
                break;
            case NORTH:
                tempC = c - rowSize;
                pDif = tempC - org;
                if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0) { //Check for if withing boundries and available space
                    genDir[NORTH] = true;
                    (*nrOfRooms)--;
                    generateRoom(org, tempC, wSize, nrOfRooms, dir);
                }
                break;
            case EAST:
                tempC = c + 1;
                pDif = tempC - org;
                if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0 && !((pDif % rowSize) == 0)) { //Check extra for end of row
                    genDir[EAST] = true;
                    (*nrOfRooms)--;
                    generateRoom(org, tempC, wSize, nrOfRooms, dir);
                }
                break;
            case SOUTH:
                tempC = c + rowSize;
                pDif = tempC - org;
                if(pDif >= 0 && pDif < *wSize && tempC->tileType[0][0] == 0) { //Check for if withing boundries and available space
                    genDir[SOUTH] = true;
                    (*nrOfRooms)--;
                    generateRoom(org, tempC, wSize, nrOfRooms, dir);
                }
                break;
        }

        hop += 2.5;
        dir = (int)hop % 4;
    }

    switch(SDL_rand(ROOM_TYPES)) {
        case TEST:
            for(int y = 0; y < CHUNK_SIZE; y++) {
                for(int x = 0; x < CHUNK_SIZE; x++) {
                    if(!x || x == (CHUNK_SIZE-1) || !y || y == (CHUNK_SIZE-1)) {
                        c->tileType[y][x] = WALL;
                    } else {
                        c->tileType[y][x] = FLOOR;
                    }
                }
            }
            
            for(int i = 0; i < 4; i++) {
                if(genDir[i]) {
                    for(int j = 0; j < CHUNK_SIZE; j++) {
                        if(j <= ((int)(CHUNK_SIZE/2) + (int)(CHUNK_SIZE/32)) && j >= ((int)(CHUNK_SIZE/2) -1 -(int)(CHUNK_SIZE/32))) {
                            switch(i) {
                                case WEST:
                                    c->tileType[j][0] = 1;
                                    break;
                                case NORTH:
                                    c->tileType[0][j] = 1;
                                    break;
                                case EAST:
                                    c->tileType[j][CHUNK_SIZE-1] = 1;
                                    break;
                                case SOUTH:
                                    c->tileType[CHUNK_SIZE-1][j] = 1;
                                    break;  
                            }
                        }
                    }
                }
            }
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
    generateRoom(w->chunks, temp, &(w->size), nrOfRooms, 5);

    SDL_Log("---------------------");
}

void polishDungeon(World w) { //Fix tileset in dungeon
    const int rowSize = (int)SDL_sqrt(w->size);
    Chunk* chunks = w->chunks;
    Chunk* tempC;
    bool dir[4] = {0};

    Chunk* saveChunks = SDL_calloc(w->size, sizeof(Chunk));
    Chunk* tempS = saveChunks;
    cleanChunks(saveChunks, w->size);


    for(int i = 0; i < w->size; i++) {
        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++) {
                if(chunks->tileType[y][x] != 0) {
                    for(int j = 0; j < 4; j++) {
                        switch(j) {
                            case WEST:
                                tempC = chunks - 1;

                                if((x > 0 && tempC->tileType[y][x] == chunks->tileType[y][x]) || (x == 0 && tempC >= w->chunks && tempC->tileType[y][x] == chunks->tileType[y][x])) {
                                    dir[WEST] == true;
                                } else {
                                    dir[WEST] == false;
                                }
                                break;
                            case NORTH:
                                tempC = chunks - rowSize;

                                if((y > 0 && tempC->tileType[y][x] == chunks->tileType[y][x]) || (y == 0 && tempC >= w->chunks && tempC->tileType[y][x] == chunks->tileType[y][x])) {
                                    dir[NORTH] == true;
                                } else {
                                    dir[NORTH] == false;
                                }
                                break;
                            case EAST:
                                tempC = chunks + 1;

                                if((x < (CHUNK_SIZE-1) && tempC->tileType[y][x] == chunks->tileType[y][x]) || (x == (CHUNK_SIZE-1) && tempC >= w->chunks && tempC->tileType[y][x] == chunks->tileType[y][x])) {
                                    dir[EAST] == true;
                                } else {
                                    dir[EAST] == false;
                                }
                                break;
                            case SOUTH:
                                tempC = chunks + rowSize;

                                if((y < (CHUNK_SIZE-1) && tempC->tileType[y][x] == chunks->tileType[y][x]) || (y == (CHUNK_SIZE-1) && tempC >= w->chunks && tempC->tileType[y][x] == chunks->tileType[y][x])) {
                                    dir[SOUTH] == true;
                                } else {
                                    dir[SOUTH] == false;
                                }
                                break;
                        }
                    }

                    if(chunks->tileType[y][x] == FLOOR) {
                        // if(dir[WEST] && dir[NORTH] && dir[EAST] && dir[SOUTH]) {
                        //     saveChunks->tileType[y][x] == 10;
                        // }

                        tempS->tileType[y][x] = 10;
                    } else if(chunks->tileType[y][x] == WALL) {
                        // if(dir[WEST] && dir[NORTH] && dir[EAST] && dir[SOUTH]) {
                        //     saveChunks->tileType[y][x] == 56;
                        // }

                        tempS->tileType[y][x] = 56;
                        // SDL_Log("%d", saveChunks->tileType[y][x]);
                    }
                }
            }
        }
        chunks++;
        tempS++;
    }

    SDL_free(w->chunks);
    w->chunks = saveChunks;
}

void createDungeon(World w, Uint8 nrOfRooms) {
    generateDungeon(w, &nrOfRooms);
    polishDungeon(w);

}

bool renderDungeon(AppState state) {
    int yLevel = 0;
    Chunk* tempC = state->world->chunks;
    const int rowSize = (int)SDL_sqrt(state->world->size);
    SDL_FRect srcRect = {0,0,TILE_SIZE,TILE_SIZE}, dstRect = {0,0,TILE_SIZE*RENDER_SCALE,TILE_SIZE*RENDER_SCALE};
    
    for(int i = 0; i < state->world->size; i++) {
        if(i % rowSize == 0 && i) { //Calc to change to next row, only counts when i is not 0
            yLevel++;
        }

        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++) {
                dstRect.x = state->camera.x + state->players[0].pos.x + (x+CHUNK_SIZE*((tempC - state->world->chunks) % rowSize))*TILE_SIZE*RENDER_SCALE - rowSize*CHUNK_SIZE*TILE_SIZE*RENDER_SCALE/2;
                dstRect.y = state->camera.y + state->players[0].pos.y + (y+CHUNK_SIZE*((int)(tempC - state->world->chunks) / rowSize))*TILE_SIZE*RENDER_SCALE - rowSize*CHUNK_SIZE*TILE_SIZE*RENDER_SCALE/2;

                if(dstRect.x >= -(TILE_SIZE*RENDER_SCALE) && dstRect.y >= -(TILE_SIZE*RENDER_SCALE) && dstRect.x <= state->displayMode->w && dstRect.y <= state->displayMode->h && tempC->tileType[y][x] != 0) {
                    srcRect.x = TILE_SIZE*((int)(tempC->tileType[y][x]%10));
                    srcRect.y = TILE_SIZE*((int)(tempC->tileType[y][x]/10));

                    if(!SDL_RenderTexture(state->renderer, state->world->texture, &srcRect, &dstRect)) {
                        SDL_Log("TILE RENDER ERROR: %d : %s", tempC->tileType[y][x], SDL_GetError());
                        return 0;
                    }
                }
            }
        }

        tempC++;
    }

    return 1;
}