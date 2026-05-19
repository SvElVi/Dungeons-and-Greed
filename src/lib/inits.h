#ifndef INITS_H
#define INITS_H

#include <SDL3_net/SDL_net.h>

#ifndef DEBUG
#define DEBUG 1
#endif

#define MAX_PLAYERS 2
#define PLAYER_SIZE 48
#define RENDER_SCALE 4
#define ANIMATION_TIME 4
#define PLAYER_NAME_MAX 30

#define MAX_ENEMIES 10
#define SKELETON_SIZE 32
#define ENEMY_AGGRO_RANGE 400
#define ENEMY_ATTACK_RANGE 28

typedef struct
{
    int x, y;
} Vector2D;

typedef enum
{
    WEST,
    NORTH,
    EAST,
    SOUTH
} direction;

typedef enum
{
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
    ENUM_MARGIN_PLAYER,
    ENUM_MARGIN_SKELETON
} SpriteMargins;

typedef enum
{
    CLASS_NONE, // 0
    CLASS_MAGE,
    CLASS_PRIEST,
    CLASS_HUNTER,
    CLASS_SWORDMASTER,
    CLASS_KNIGHT // 5
} Player_Class;

typedef struct world *World;

typedef struct
{
    const char *menuOptions[3];
    int selected;
    int count;
} Menu;

typedef struct
{
    int health;
    int maxHealth;
    int mana;
    int defense;
    int attackPower;
    int attackSpeed;
} Stats;

typedef struct
{
    int moveX;
    int moveY;
} Player_Flags;

typedef struct
{
    int playerID;
    Vector2D pos;         ////SYNC MULTIPLAYER, x & y axis is inverted so these will be values from 0 to below.
    Player_Flags flags;   // SYNC MULTIPLAYER
    Player_Class class;   // SYNC MULTIPLAYER
    Stats stats;          // SYNC MULTIPLAYER
    SDL_Texture *texture; // LOCAL
    SDL_FRect hitBox;
    SDL_FRect aniBox;           // LOCAL
    direction facing;           // SYNC MULTIPLAYER
    SDL_FlipMode flip;          // SYNC MULTIPLAYER
    char name[PLAYER_NAME_MAX]; // player name string
    Uint32 enemyCollisionTimer; // ms counter for character colliding with emeny. SYNC MULTIPLAYER

    int connected;
    bool classLock; // För att låsa klasser LOCAL
    // FOR SERVER
    NET_Address *ipAddress;
} Player;

typedef enum
{
    ENEMY_SKELETON,
} Enemy_Type;

typedef enum
{
    ENEMY_IDLE,
    ENEMY_CHASE,
    ENEMY_ATTACK,
    ENEMY_DEAD
} Enemy_State;

typedef struct
{
    Vector2D pos;
    SDL_FRect hitBox;
    SDL_FRect aniBox;
    SDL_Texture *texture;
    SDL_Texture *texIdle;
    SDL_Texture *texMove;
    SDL_Texture *texAttack;
    SDL_Texture *texTakeDamage;
    SDL_Texture *texDeath;
    SDL_FlipMode flip;
    direction facing;

    Enemy_Type type;
    Enemy_State state;
    Stats stats;

    int moveX;
    int moveY;
    char sizeModifier;
} Enemy;

typedef struct
{
    Vector2D location;

} PlayerLocations;

typedef struct
{
    int amountOfPlayers;
    int playerID[MAX_PLAYERS];
    char name[MAX_PLAYERS][PLAYER_NAME_MAX];
    bool connected[MAX_PLAYERS];
    bool ready[MAX_PLAYERS];

    Player_Class class[MAX_PLAYERS];
} LobbyPacket;

typedef enum GameState
{
    GAME_MENY,
    GAME_LOBBY,
    GAME_JOIN,
    GAME_INIT,
    GAME_NET_INIT,
    GAME_IP_INIT,
    GAME_IP_INIT_CHECK,
    GAME_TCP_INIT,
    GAME_HANDSHAKE,
    GAME_VERIFYING_HANDSHAKE,
    GAME_WAITING_FOR_OTHER_PLAYERS,
    GAME_GENERATE_WORLD,
    GAME_START,
    GAME_PLAYING,
    GAME_UPDATE_MY_LOCATION,
    GAME_PAUSE,
    GAME_OVER,
    GAME_SERVER_SHUTDOWN,
    GAME_DEAD,
    SERVER
} GameState;

// Serverside state
typedef enum
{
    INIT_OF_SERVER,
    WAITING_FOR_PLAYERS,
    ASSIGNING_PLAYER_ID,
    SENDING_PLAYER_ID,
    CONFIRMING_PLAYER_ID_RECIVE,
    UPDATE_WAITING_STATUS,
    LOBBY,
    STARTING_GAME,
    GAME_ONGOING,
    UPDATE_PLAYERS_SERVERSIDE,
    BROADCASTING_PLAYERS_TO_CLIENTS,
    SERVER_CLEANUP
} ServerState;

struct networkInterface;

typedef struct networkInterface *NetworkInterface;

struct enemies;

typedef struct enemies *Enemies;

struct appState
{
    SDL_Window *window;                 // LOCAL
    SDL_Renderer *renderer;             // LOCAL
    SDL_DisplayID displayID;            // LOCAL
    const SDL_DisplayMode *displayMode; // LOCAL
    float framerate;                    // LOCAL
    Uint64 deltaTime;                   // LOCAL
    Uint64 lastTime;                    // LOCAL
    ServerState serverState;
    GameState gameState;
    bool onlineMode;
    int amountOfPlayers;
    Menu mainMenu;

    Player players[MAX_PLAYERS]; // SEE STRUCT
    SDL_FRect camera;            // LOCAL
    Player *curPlayerPtr;        // LOCAL

    Enemy enemies[MAX_ENEMIES];
    Enemies enemiesADT;

    Uint8 animationTime;      // LOCAL
    Uint8 enemyAnimationTime; // LOCAL
    bool running;             // LOCAL
    bool computedEvent;       // LOCAL

    // WORLD
    World world; // LOCAL
    int seed;

    char hostIP[16];
    int hostIPLen;

    NetworkInterface ptrNetworkInterface;
};

typedef struct appState *AppState;

AppState createAppState(void);

int initDisplay(AppState state);

void initCam(AppState state);

#endif