#include <SDL3/SDL.h>
#define MAX_PLAYERS 5

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
    Player_Flags flags;
    SDL_FRect renderBox;
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
    SDL_Rect displaySize;
    Uint64 deltaTime;
    Uint64 lastTime;
    int renderFlag;
    Client clients [MAX_PLAYERS];
    Player players [MAX_PLAYERS];

    bool running;
    //bool computedEvent;
} AppState;

int initDisplay(AppState* state);

void initArt(AppState* state);