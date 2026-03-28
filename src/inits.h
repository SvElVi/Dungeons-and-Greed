typedef enum {
    CLASS_NONE, //0
    CLASS_MAGE,
    CLASS_PRIEST,
    CLASS_HUNTER,
    CLASS_SWORDMASTER,
    CLASS_KNIGHT //5
} Player_Class;

typedef struct {
    int x;
    int y;
} Vector2D;

typedef struct {
    int health;
    int mana;
    int defense;
    int attackPower;
    int attackSpeed;
} Stats;

typedef struct {
    Vector2D pos;
    Player_Class class;
    Stats stats;
} Player;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayID displayID;
    SDL_Rect displaySize;

    bool running;
} AppState;

int initDisplay(AppState* state);