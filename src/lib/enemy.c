#include "enemy.h"
#include <math.h>

#define ENEMY_SPEED 0.15f * RENDER_SCALE
#define ENEMY_FRAMES 4

#define SKELETON_IDLE_FRAMES 6
#define SKELETON_MOVE_FRAMES 10
#define SKELETON_ATTACK_FRAMES 9
#define SKELETON_DAMAGE_FRAMES 5
#define SKELETON_DEATH_FRAMES 17

struct enemies {
    int amountOfEnemies;
    Enemy enemies[MAX_ENEMIES];
};

Enemies createEnemies() {
    Enemies ptr = SDL_calloc(1, sizeof(struct enemies));
    if (ptr != NULL) {
        SDL_Log("Created Enemies ADT!\n");
    }
    return ptr;
}

void destoryEnemies(Enemies ptrEnemies) {
    SDL_free(ptrEnemies);
}


int nearestPlayer(Enemy *enemy, Player players[MAX_PLAYERS])
{
    int nearest = -1;
    float nearestDist = ENEMY_AGGRO_RANGE * ENEMY_AGGRO_RANGE;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        float dx = enemy->pos.x - players[i].pos.x;
        float dy = enemy->pos.y - players[i].pos.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < nearestDist)
        {
            nearestDist = distSq;
            nearest = i;
        }
    }

    return nearest;
}

static void syncEnemyHitbox(Enemy *enemy)
{
    float hbW = enemy->hitBox.w;
    float hbH = enemy->hitBox.h;

    float offsetX = (ENEMY_SPRITE_SIZE * RENDER_SCALE * 1.5f - hbW) / 2.7f;
    float offsetY = (ENEMY_SPRITE_SIZE * RENDER_SCALE * 1.5f - hbH) / 1.1f;

    enemy->hitBox.x = enemy->pos.x + offsetX;
    enemy->hitBox.y = enemy->pos.y + offsetY;
}

void enemyMovement(Enemy *enemy, Player players[MAX_PLAYERS], int deltatime, World world)
{
    int target = nearestPlayer(enemy, players);

    if (target == -1)
    {
        enemy->state = ENEMY_IDLE;
        syncEnemyHitbox(enemy);
        return;
    }

    float dx = players[target].pos.x - enemy->pos.x;
    float dy = players[target].pos.y - enemy->pos.y;
    float distSq = dx * dx + dy * dy;

    if (distSq < (float)(ENEMY_ATTACK_RANGE * ENEMY_ATTACK_RANGE))
    {
        enemy->state = ENEMY_ATTACK;
        syncEnemyHitbox(enemy);
        return;
    }

    enemy->state = ENEMY_CHASE;

    float dist = SDL_sqrtf(distSq);
    float normX = dx / dist;
    float normY = dy / dist;

    float speed = ENEMY_SPEED;
    float nextX = enemy->pos.x + normX * deltatime * speed;
    float nextY = enemy->pos.y + normY * deltatime * speed;

    SDL_FRect futureHitBox = enemy->hitBox;
    futureHitBox.x = nextX;
    futureHitBox.y = enemy->pos.y;

    if (!tileCollision(world, futureHitBox, NULL, enemy))
    {
        enemy->pos.x = nextX;
    }

    futureHitBox.x = enemy->pos.x;
    futureHitBox.y = nextY;

    if (!tileCollision(world, futureHitBox, NULL, enemy))
    {
        enemy->pos.y = nextY;
    }

    syncEnemyHitbox(enemy);
}

void animateEnemies(Enemy enemies[MAX_ENEMIES], Uint8 *counter, Uint16 framerate, bool *flag)
{
    ++*counter;
    if ((*counter) < (framerate / ANIMATION_TIME / 2))
        return;
    *counter = 0;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        Enemy *e = &enemies[i];
        if (e->texture == NULL)
            continue;

        SDL_Texture *newTex = e->texIdle;
        int maxFrames = SKELETON_IDLE_FRAMES;

        switch (e->state)
        {
        case ENEMY_CHASE:
            newTex = e->texMove;
            maxFrames = SKELETON_MOVE_FRAMES;
            break;
        case ENEMY_ATTACK:
            newTex = e->texAttack;
            maxFrames = SKELETON_ATTACK_FRAMES;
            break;
        case ENEMY_DEAD:
            newTex = e->texDeath;
            maxFrames = SKELETON_DEATH_FRAMES;
            break;
        case ENEMY_IDLE:
        default:
            newTex = e->texIdle;
            maxFrames = SKELETON_IDLE_FRAMES;
            break;
        }

        if (e->texture != newTex)
        {
            e->texture = newTex;
            e->aniBox.x = 0;
        }

        int currentFrame = (int)e->aniBox.x / ENEMY_SPRITE_SIZE;

        if (e->state == ENEMY_DEAD)
        {
            if (currentFrame < maxFrames - 1)
            {
                e->aniBox.x += ENEMY_SPRITE_SIZE;
            }
        }
        else
        {
            e->aniBox.x += ENEMY_SPRITE_SIZE;
            if ((int)e->aniBox.x >= ENEMY_SPRITE_SIZE * maxFrames)
            {
                e->aniBox.x = 0;
            }
        }
    }
}

static void updateEnemyClass(Enemy *enemy, SDL_Renderer *renderer)
{
    SDL_Surface *s;

#define LOAD_AND_CHECK(tex, path)                                          \
    s = SDL_LoadPNG(path);                                                 \
    if (!s)                                                                \
    {                                                                      \
        SDL_Log("MISSING TEXTURE: %s - %s", path, SDL_GetError());         \
        return;                                                            \
    }                                                                      \
    tex = SDL_CreateTextureFromSurface(renderer, s);                       \
    SDL_DestroySurface(s);                                                 \
    if (!tex)                                                              \
    {                                                                      \
        SDL_Log("TEXTURE CREATION FAILED: %s - %s", path, SDL_GetError()); \
        return;                                                            \
    }

    switch (enemy->type)
    {

    case ENEMY_SKELETON:
        s = SDL_LoadPNG("img/Custom/enemies-skeleton1_idle.png");
        enemy->texIdle = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);

        s = SDL_LoadPNG("img/Custom/enemies-skeleton1_movement.png");
        enemy->texMove = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);

        s = SDL_LoadPNG("img/Custom/enemies-skeleton1_attack.png");
        enemy->texAttack = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);

        s = SDL_LoadPNG("img/Custom/enemies-skeleton1_take_damage.png");
        enemy->texTakeDamage = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);

        s = SDL_LoadPNG("img/Custom/enemies-skeleton1_death.png");
        enemy->texDeath = SDL_CreateTextureFromSurface(renderer, s);
        SDL_DestroySurface(s);
        break;
    }
    enemy->texture = enemy->texIdle;
}

void updateEnemy(Enemy *enemy, Vector2D pos, Enemy_Type type, Stats stats, SDL_Renderer *renderer)
{

    enemy->aniBox.w = ENEMY_SPRITE_SIZE;
    enemy->aniBox.h = ENEMY_SPRITE_SIZE;
    enemy->aniBox.x = 0;
    enemy->aniBox.y = 0;

    enemy->hitBox.w = 12 * RENDER_SCALE;
    enemy->hitBox.h = 5 * RENDER_SCALE;

    enemy->pos = pos;
    syncEnemyHitbox(enemy);
    enemy->stats = stats;
    
    if (enemy->stats.maxHealth <= 0)
        enemy->stats.maxHealth = 100;
    if (enemy->stats.health <= 0)
        enemy->stats.health = enemy->stats.maxHealth;

    enemy->state = ENEMY_IDLE;
    enemy->facing = SOUTH;
    updateEnemyClass(enemy, renderer);
}