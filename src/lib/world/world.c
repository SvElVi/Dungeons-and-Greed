#include <SDL3/SDL.h>
#include "../inits.h"
#include "rooms.h"

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

void generateConnections(Chunk* c, bool genDir[4]) {
    const int startpos = ((int)(CHUNK_SIZE/2) -2 -(int)(CHUNK_SIZE/32));
    bool stop = false;

    if(genDir[WEST]) {
        for(int y = 0; y < ((int)(CHUNK_SIZE/16)+3); y++) {
            for(int x = 0; x < CHUNK_SIZE/2; x++) {
                if(c->tileType[startpos+y][x] == BLANK) {

                    if(y != 0 && y != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[startpos+y][x] = FLOOR;
                    } else {
                        c->tileType[startpos+y][x] = WALL;
                    }
                } else if(c->tileType[startpos+y][x] == WALL) {

                    if(y != 0 && y != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[startpos+y][x] = FLOOR;
                    }
                    break;
                }
            }
        }
    }

    if(genDir[NORTH]) {
        for(int y = 0; y < CHUNK_SIZE/2; y++) {
            for(int x = 0; x < ((int)(CHUNK_SIZE/16)+3); x++) {
                if(c->tileType[y][startpos+x] == BLANK) {

                    if(x != 0 && x != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[y][startpos+x] = FLOOR;
                    } else {
                        c->tileType[y][startpos+x] = WALL;
                    }
                } else if(c->tileType[y][startpos+x] == WALL) {

                    if(x != 0 && x != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[y][startpos+x] = FLOOR;
                    }
                    stop = true;
                }
            }
            if(stop) {
                break;
            }
        }
        stop = false;
    }

    if(genDir[EAST]) {
        for(int y = 0; y < ((int)(CHUNK_SIZE/16)+3); y++) {
            for(int x = 0; x < CHUNK_SIZE/2; x++) {
                if(c->tileType[startpos+y][(CHUNK_SIZE-1)-x] == BLANK) {

                    if(y != 0 && y != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[startpos+y][(CHUNK_SIZE-1)-x] = FLOOR;
                    } else {
                        c->tileType[startpos+y][(CHUNK_SIZE-1)-x] = WALL;
                    }
                } else if(c->tileType[startpos+y][(CHUNK_SIZE-1)-x] == WALL) {

                    if(y != 0 && y != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[startpos+y][(CHUNK_SIZE-1)-x] = FLOOR;
                    }
                    break;
                }
            }
        }
    }

    if(genDir[SOUTH]) {
        for(int y = 0; y < CHUNK_SIZE/2; y++) {
            for(int x = 0; x < ((int)(CHUNK_SIZE/16)+3); x++) {
                if(c->tileType[(CHUNK_SIZE-1)-y][startpos+x] == BLANK) {

                    if(x != 0 && x != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[(CHUNK_SIZE-1)-y][startpos+x] = FLOOR;
                    } else {
                        c->tileType[(CHUNK_SIZE-1)-y][startpos+x] = WALL;
                    }
                } else if(c->tileType[(CHUNK_SIZE-1)-y][startpos+x] == WALL) {

                    if(x != 0 && x != ((int)(CHUNK_SIZE/16)+2)) {
                        c->tileType[(CHUNK_SIZE-1)-y][startpos+x] = FLOOR;
                    }
                    stop = true;
                }
            }
            if(stop) {
                break;
            }
        }
        stop = false;
    }
}

void generateMirroredRoom(Chunk *c, const Uint64* room, bool genDir[4], bool horizontal, bool vertical) {
    int back, tilestep = 0, n;
    Uint64 rowData = 1; //Information of a whole data row (+1 row keeping in mind the data is mirrored across 4 pieces)
    Uint8 currentData; //Two hex value, left value for tile id and right for number of tiles.
    Uint8 size = 24; //The size described by the data

    if(!horizontal) {
        size*2;
    }

    if(!vertical) {
        size*2;
    }


    for(int i = 0; i < size && rowData; i++) { //Stop if a row is fully empty (use 0x10 to mark empty but still continue)
        rowData = room[i];
        currentData = 1;

        back = 0;
        do { //Assign last valid data row from index
            rowData = room[i-back];
            back++;

        } while(rowData == 0xF0 && back <= i);

        currentData = rowData & 0xFF;
        for(int j = 1; j < 8 && currentData > 0; j++) { //Limit 8 because that is how many steps can be taken through Uint64

            for(n = 0; n < (currentData & 0xF); n++) {

                //Mirroring below
                c->tileType[(int)((tilestep+n)/size)][(int)((tilestep+n)%size)] = currentData >> 0x4; //Left upper quarter

                if(horizontal) {
                    c->tileType[(int)((tilestep+n)/size)][(CHUNK_SIZE-1)-(int)((tilestep+n)%size)] = currentData >> 0x4; //Right upper quarter
                }

                if(vertical) {
                    c->tileType[(CHUNK_SIZE-1)-(int)((tilestep+n)/size)][(int)((tilestep+n)%size)] = currentData >> 0x4; //Left lower quarter
                    if(horizontal) {
                        c->tileType[(CHUNK_SIZE-1)-(int)((tilestep+n)/size)][(CHUNK_SIZE-1)-(int)((tilestep+n)%size)] = currentData >> 0x4; //Right lower quarter
                    }
                }
            }
            tilestep += n;

            currentData = rowData >> (0x8*j) & 0xFF; //Move to steps to the right for every step j
        }
    }
    generateConnections(c, genDir);

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
            break;
        case ROOM_CIRCLE: //Double mirrored room
            generateMirroredRoom(c, CircleRoom, genDir, 1, 1);
            break;
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

                                if((x > 0 && chunks->tileType[y][x] == chunks->tileType[y][x-1]) || (x == 0 && tempC >= w->chunks && chunks->tileType[y][x] == tempC->tileType[y][CHUNK_SIZE-1])) {
                                    dir[WEST] = true;
                                } else {
                                    dir[WEST] = false;
                                }
                                break;
                            case NORTH:
                                tempC = chunks - rowSize;

                                if((y > 0 && chunks->tileType[y][x] == chunks->tileType[y-1][x]) || (y == 0 && tempC >= w->chunks && chunks->tileType[y][x] == tempC->tileType[CHUNK_SIZE-1][x])) {
                                    dir[NORTH] = true;
                                } else {
                                    dir[NORTH] = false;
                                }
                                break;
                            case EAST:
                                tempC = chunks + 1;

                                if((x < (CHUNK_SIZE-1) && chunks->tileType[y][x] == chunks->tileType[y][x+1]) || (x == (CHUNK_SIZE-1) && tempC >= w->chunks && chunks->tileType[y][x] == tempC->tileType[y][0])) {
                                    dir[EAST] = true;
                                } else {
                                    dir[EAST] = false;
                                }
                                break;
                            case SOUTH:
                                tempC = chunks + rowSize;

                                if((y < (CHUNK_SIZE-1) && chunks->tileType[y][x] == chunks->tileType[y+1][x]) || (y == (CHUNK_SIZE-1) && tempC >= w->chunks && chunks->tileType[y][x] == tempC->tileType[0][x])) {
                                    dir[SOUTH] = true;
                                } else {
                                    dir[SOUTH] = false;
                                }
                                break;
                        }
                    }

                    if(chunks->tileType[y][x] == FLOOR) {
                        if(!dir[WEST] && !dir[NORTH] && dir[EAST] && dir[SOUTH]) { //Left upper corner
                            tempS->tileType[y][x] = 1;
                        } else if(dir[WEST] && !dir[NORTH] && dir[EAST] && dir[SOUTH]) { //Upper side
                            tempS->tileType[y][x] = 2 + SDL_rand(6);
                        } else if(dir[WEST] && !dir[NORTH] && !dir[EAST] && dir[SOUTH]) { //Right upper corner
                            tempS->tileType[y][x] = 8;
                        } else if(!dir[WEST] && dir[NORTH] && dir[EAST] && dir[SOUTH]) { //Left side
                            tempS->tileType[y][x] = 9;
                        } else if(dir[WEST] && dir[NORTH] && dir[EAST] && dir[SOUTH]) { //Middle
                            tempS->tileType[y][x] = 10 + SDL_rand(18);
                        } else if(dir[WEST] && dir[NORTH] && !dir[EAST] && dir[SOUTH]) { //Right side
                            tempS->tileType[y][x] = 28;
                        } else if(!dir[WEST] && dir[NORTH] && dir[EAST] && !dir[SOUTH]) { //Left bottom corner
                            tempS->tileType[y][x] = 29;
                        } else if(dir[WEST] && dir[NORTH] && dir[EAST] && !dir[SOUTH]) { //Bottom side
                            tempS->tileType[y][x] = 30 + SDL_rand(2);
                        } else if(dir[WEST] && dir[NORTH] && !dir[EAST] && !dir[SOUTH]) { //Right bottom corner
                            tempS->tileType[y][x] = 32;
                        } else {
                            tempS->tileType[y][x] = 40;
                        }

                        // SDL_Log("Failed: W: %d, N: %d, E: %d, S: %d", dir[WEST], dir[NORTH], dir[EAST], dir[SOUTH]);

                    } else if(chunks->tileType[y][x] == WALL) {
                         if (dir[WEST] && dir[EAST]) { //North wall
                            if((y < (CHUNK_SIZE-1) && chunks->tileType[y+1][x] == FLOOR) || (y == 0 && tempC >= w->chunks && tempC->tileType[y][x] == FLOOR)) { //Point down wall
                                tempS->tileType[y][x] = 56 + SDL_rand(3);
                            } else if((y > 0 && chunks->tileType[y-1][x] == FLOOR) || (y == (CHUNK_SIZE-1) && tempC >= w->chunks && tempC->tileType[y][x] == FLOOR)) { //Point up wall
                                tempS->tileType[y][x] = 42 + SDL_rand(6);
                            } else if(y > 0 && x < (CHUNK_SIZE-1) && chunks->tileType[y-1][x+1] == FLOOR) { //Bottom left corner
                                tempS->tileType[y][x] = 48;
                            } else if(y > 0 && x > 0 && chunks->tileType[y-1][x-1] == FLOOR) { //Bottom right corner
                                tempS->tileType[y][x] = 41;
                            } else if(y < (CHUNK_SIZE-1) && x < (CHUNK_SIZE-1) && chunks->tileType[y+1][x+1] == FLOOR) { //Right wall
                                tempS->tileType[y][x] = 53 + SDL_rand(3);
                            } else if(y < (CHUNK_SIZE-1) && x > 0 && chunks->tileType[y+1][x-1] == FLOOR) { //Left wall
                                tempS->tileType[y][x] = 49 + SDL_rand(3);
                            } else { //Debug
                                tempS->tileType[y][x] = 40;
                            }
                            
                            
                        } else if(dir[NORTH] && dir[SOUTH]) {
                            if((x < (CHUNK_SIZE-1) && chunks->tileType[y][x+1] == FLOOR) || (x == 0 && tempC >= w->chunks && tempC->tileType[y][x] == FLOOR)) {
                                tempS->tileType[y][x] = 53 + SDL_rand(3);
                            } else if((y > 0 && chunks->tileType[y][x-1] == FLOOR) || (x == (CHUNK_SIZE-1) && tempC >= w->chunks && tempC->tileType[y][x] == FLOOR)) {
                                tempS->tileType[y][x] = 49 + SDL_rand(3);
                            } else if(y > 0 && x < (CHUNK_SIZE-1) && chunks->tileType[y-1][x+1] == FLOOR) {
                                tempS->tileType[y][x] = 48;
                            } else if(y > 0 && x > 0 && chunks->tileType[y-1][x-1] == FLOOR) {
                                tempS->tileType[y][x] = 41;
                            } else if(y < (CHUNK_SIZE-1) && x < (CHUNK_SIZE-1) && chunks->tileType[y+1][x+1] == FLOOR) {
                                tempS->tileType[y][x] = 53 + SDL_rand(3);
                            } else if(y < (CHUNK_SIZE-1) && x > 0 && chunks->tileType[y+1][x-1] == FLOOR) {
                                tempS->tileType[y][x] = 49 + SDL_rand(3);
                            } else {
                                tempS->tileType[y][x] = 40;
                            }
                        } else {
                            if((!dir[WEST] && !dir[NORTH] && dir[SOUTH] && dir[EAST]) && (y > 0 && x > 0 && chunks->tileType[y-1][x-1] == FLOOR)){ //Upper left corner
                                tempS->tileType[y][x] = 60 + SDL_rand(2);  
                            } else if((dir[WEST] && !dir[NORTH] && dir[SOUTH] && !dir[EAST]) && (y > 0 && x < (CHUNK_SIZE-1) && chunks->tileType[y-1][x+1] == FLOOR)){ //Upper right corner
                                tempS->tileType[y][x] = 62 + SDL_rand(2);  
                            } else if((!dir[WEST] && dir[NORTH] && !dir[SOUTH] && dir[EAST]) && (y < (CHUNK_SIZE-1) && x > 0 && chunks->tileType[y+1][x-1] == FLOOR)) {
                                tempS->tileType[y][x] = 56 + SDL_rand(3);
                            } else if((dir[WEST] && dir[NORTH] && !dir[SOUTH] && !dir[EAST]) && (y < (CHUNK_SIZE-1) && x < (CHUNK_SIZE-1) && chunks->tileType[y+1][x+1] == FLOOR)) {
                                tempS->tileType[y][x] = 56 + SDL_rand(3);
                            } else if(y > 0 && x < (CHUNK_SIZE-1) && chunks->tileType[y-1][x+1] == FLOOR) {
                                tempS->tileType[y][x] = 48;
                            } else if(y > 0 && x > 0 && chunks->tileType[y-1][x-1] == FLOOR) {
                                tempS->tileType[y][x] = 41;
                            } else if(y < (CHUNK_SIZE-1) && x < (CHUNK_SIZE-1) && chunks->tileType[y+1][x+1] == FLOOR) {
                                tempS->tileType[y][x] = 53 + SDL_rand(3);
                            } else if(y < (CHUNK_SIZE-1) && x > 0 && chunks->tileType[y+1][x-1] == FLOOR) {
                                tempS->tileType[y][x] = 49 + SDL_rand(3);
                            }  else {
                                tempS->tileType[y][x] = 40;
                            }
                        }
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
                    srcRect.x = TILE_SIZE*((int)((tempC->tileType[y][x]-1)%10));
                    srcRect.y = TILE_SIZE*((int)((tempC->tileType[y][x]-1)/10));

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