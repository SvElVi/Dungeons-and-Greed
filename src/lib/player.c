#include "inits.h"
#include "player.h"
#define SPEED 0.25*RENDER_SCALE

bool collision(SDL_FRect a, SDL_FRect b)    //beräkna rektangel a med rektangel b
{                                           //Standard kollisionsberäkning för AABB(fyrkantskollisiondetektion)
    if(a.x < b.x + b.w &&
    a.x +a.w > b.x &&
    a.y < b.y + b.h &&
    a.y + a.h > b.y)
    {
        return true;
    }
    else
    {   
        return false; 
    }
}

bool willCollide(Player* player, Player players[MAX_PLAYERS], Enemy enemies[MAX_ENEMIES], float futureX, float futureY){
    SDL_FRect futurePos = player->hitBox; //Testar framtida position för att minska buggar
    futurePos.x = futureX;
    futurePos.y = futureY;
    
    for(int i = 0; i < MAX_PLAYERS;i++)
    {
        if(&players[i] == player)continue;  //Hoppa dig själv
        if(collision(futurePos, players[i].hitBox)) //Kolla din framtida position med hitbox av andra spelare
            return true;
    }
    for(int i = 0; i < MAX_ENEMIES;i++)
    {
        if(collision(futurePos, enemies[i].hitBox)){
            return true;
        }     
    }
    return false;
}

void movement(Player* player, Player players[MAX_PLAYERS], Enemy enemies[MAX_ENEMIES], int deltatime) {

    if(player->flags.moveX != 0){
        if(!willCollide(player, players, enemies, player->pos.x - player->flags.moveX * deltatime * SPEED, player->pos.y)) //testa ny x-position 
        {
            player->pos.x -= player->flags.moveX * deltatime * SPEED;
        }
        player->facing = player->flags.moveX + 1;
    }
    if(player->flags.moveY != 0){
        if(!willCollide(player, players, enemies, player->pos.x , player->pos.y - player->flags.moveY * deltatime * SPEED)) //testa ny y-position
        {
            player->pos.y -= player->flags.moveY * deltatime * SPEED;
        }
        player->facing = player->flags.moveY + 2;
    }
    player->hitBox.x = player->pos.x;
    player->hitBox.y = player->pos.y;

} 

bool playerEnemyCollision(Player* player, Enemy enemies[MAX_ENEMIES], Uint32 deltatime){
    bool playerEnemyCollided = false;

    for (int i = 0; i < MAX_ENEMIES; i++){
        SDL_FRect dotDmgHitBox = enemies[i].hitBox;

        dotDmgHitBox.x -= RENDER_SCALE;
        dotDmgHitBox.y -= RENDER_SCALE;
        dotDmgHitBox.w += RENDER_SCALE*2;
        dotDmgHitBox.h += RENDER_SCALE*2;

        if(collision(player->hitBox, dotDmgHitBox)){
            playerEnemyCollided = true;
            break;
        }
    }

    if(playerEnemyCollided){
        player->enemyCollisionTimer += deltatime;

        while(player->enemyCollisionTimer >= 1000){
            if(player->stats.health > 0){
                player->stats.health -= 10;
            }
            else player->stats.health = 100;

            player->enemyCollisionTimer -= 1000;
        }
    }
    else player->enemyCollisionTimer = 0;

    return playerEnemyCollided;
}

 //&& (player->flags.moveX || player->flags.moveY)
 // || 
void animatePlayers(Player players[MAX_PLAYERS], Uint8* counter, Uint16 framerate, bool* flag) {
    ++*counter;
    if((*counter) == (framerate / ANIMATION_TIME /2) || (*counter) == (framerate / (ANIMATION_TIME))) {
        for(int i = 0; i < MAX_PLAYERS; i++) {
            if(players[i].flags.moveX || players[i].flags.moveY) { //Render faster if moving
                switch(players[i].facing) {
                    case WEST:
                        players[i].aniBox.y = WALK_HORIZONTAL * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_HORIZONTAL;
                        break;
                    case NORTH:
                        players[i].aniBox.y = WALK_NORTH * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                    case EAST:
                        players[i].aniBox.y = WALK_HORIZONTAL * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                    case SOUTH:
                        players[i].aniBox.y = WALK_SOUTH * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                }

                players[i].aniBox.x += PLAYER_SIZE;
                players[i].aniBox.x = (float)((int)players[i].aniBox.x % (PLAYER_SIZE*6));

            } else if((*counter) == (framerate / ANIMATION_TIME)) {
                switch(players[i].facing) {
                    case WEST:
                        players[i].aniBox.y = IDLE_HORIZONTAL * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_HORIZONTAL;
                        break;
                    case NORTH:
                        players[i].aniBox.y = IDLE_NORTH * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                    case EAST:
                        players[i].aniBox.y = IDLE_HORIZONTAL * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                    case SOUTH:
                        players[i].aniBox.y = IDLE_SOUTH * PLAYER_SIZE;
                        players[i].flip = SDL_FLIP_NONE;
                        break;
                }

                players[i].aniBox.x += PLAYER_SIZE;
                players[i].aniBox.x = (float)((int)players[i].aniBox.x % (PLAYER_SIZE*6));
            }
        }
    }
    if((*counter) == (framerate / ANIMATION_TIME)) {
        (*counter) = 0;
    }
}

void updateClass(Player* player, SDL_Renderer* renderer) {
    SDL_Surface* pArt;
    switch (player->class) { //Paths below should be change in the future
        case CLASS_NONE:
            pArt = SDL_LoadPNG("./img/Custom/Player.png");
            break;
        case CLASS_MAGE:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_PRIEST:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_HUNTER:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_SWORDMASTER:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
        case CLASS_KNIGHT:
            pArt = SDL_LoadPNG("./img/2D Pixel Dungeon Asset Pack/Character_animation/priests_idle/priest1/v1/priest1_v1_1.png");
            break;
    }
    player->texture = SDL_CreateTextureFromSurface(renderer, pArt);
    SDL_DestroySurface(pArt);
}

void updatePlayer(Player* player, Vector2D pos, Player_Class class, Stats stats, SDL_Renderer* renderer) {
    player->aniBox.w = PLAYER_SIZE;
    player->aniBox.h = PLAYER_SIZE;
    player->hitBox.w = 9*RENDER_SCALE;
    player->hitBox.h = 2*RENDER_SCALE;
    player->aniBox.x = 0;
    player->aniBox.y = 0;
    player->pos = pos;
    player->hitBox.x = player->pos.x;
    player->hitBox.y = player->pos.y;
    player->class = class;
    player->enemyCollisionTimer = 0;
    updateClass(player, renderer);

    // temporary health reset when it goes down to 0
    if (stats.health <= 0 ) {
        stats.health = stats.maxHealth;
    }
    player->stats = stats;
    // SDL_Log("Pos: %d Class: %d Stats: %d", pos, class, stats);
}