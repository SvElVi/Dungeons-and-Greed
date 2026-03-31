#include <SDL3/SDL.h>
#define MAX_PLAYERS 5
#define RENDER_SCALE 4
#define SPRITE_SIZE 16

typedef struct {
    int x, y;
} Vector2D;

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
} Player;

typedef struct {
    int temp;
} Client;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayID displayID;
    const SDL_DisplayMode* displayMode;
    int framerate;
    Uint64 deltaTime;
    Uint64 lastTime;
    int renderFlag;
    Client clients [MAX_PLAYERS];
    Player players [MAX_PLAYERS];
    SDL_FRect camera;

    bool running;
    //bool computedEvent;
} AppState;

int initDisplay(AppState* state);

void initArt(AppState* state);