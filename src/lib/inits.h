#include <SDL3/SDL.h>
#define MAX_PLAYERS 5
#define PLAYER_SIZE 48
#define PLAYER_RENDER_SCALE 2
#define TILE_SIZE 16
#define TILE_RENDER_SCALE 2
#define ANIMATION_TIME 2

typedef struct {
    int x, y;
} Vector2D;

typedef enum {
    WEST,
    NORTH,
    EAST,
    SOUTH
} direction;

typedef enum{
    IDLE_SOUTH,
    IDLE_HORIZONTAL,
    IDLE_NORTH,
    WALK_SOUTH,
    WALK_HORIZONTAL,
    WALK_NORTH,
    ATTACK_SOUTH,
    ATTACK_HORIZONTAL,
    ATTACK_NORTH
} AniState;

typedef enum {
    CLASS_NONE, //0
    CLASS_MAGE,
    CLASS_PRIEST,
    CLASS_HUNTER,
    CLASS_SWORDMASTER,
    CLASS_KNIGHT //5
} Player_Class;

typedef struct {
    int health;
    int mana;
    int defense;
    int attackPower;
    int attackSpeed;
} Stats;

typedef struct {
    int moveX;
    int moveY;
} Player_Flags;

typedef struct {
    Vector2D pos;
    Player_Flags flags;
    Player_Class class;
    Stats stats;
    SDL_Texture* texture;

    SDL_FRect aniBox;
    Uint8 animationTime;
    direction facing;
} Player;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayID displayID;
    const SDL_DisplayMode* displayMode;
    int framerate;
    Uint64 deltaTime;
    Uint64 lastTime;
    // bool renderFlag;
    Player players [MAX_PLAYERS];
    SDL_FRect camera;

    bool running;
    //bool computedEvent;
} AppState;

int initDisplay(AppState* state);

void initCam(AppState* state);