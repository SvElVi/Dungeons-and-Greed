#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

#define MAX_PLAYERS 5
#define PLAYER_SIZE 48
#define PLAYER_RENDER_SCALE 2
#define TILE_SIZE 16
#define TILE_RENDER_SCALE 2
#define ANIMATION_TIME 4
#define CHUNK_SIZE 16

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
    Vector2D pos; ////SYNC MULTIPLAYER
    Player_Flags flags; //SYNC MULTIPLAYER
    Player_Class class; //SYNC MULTIPLAYER
    Stats stats; //SYNC MULTIPLAYER
    SDL_Texture* texture; //LOCAL

    SDL_FRect aniBox; //LOCAL
    direction facing; //SYNC MULTIPLAYER
    SDL_FlipMode flip; //SYNC MULTIPLAYER
} Player;

typedef struct {
    Uint8 tileType[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

typedef struct {
    Chunk chunks;
    Uint64 cord;
    struct XChunks* nextXChunk;
} XChunks;

typedef struct {
    XChunks* xChunks;
    Uint64 cord;
    struct YChunks* nextYChunk;
} YChunks;

typedef struct {
    YChunks quarter[4];
} World;

struct appState {
    SDL_Window *window; //LOCAL
    SDL_Renderer *renderer; //LOCAL
    SDL_DisplayID displayID; //LOCAL
    const SDL_DisplayMode* displayMode; //LOCAL
    Uint16 framerate; //LOCAL
    Uint64 deltaTime; //LOCAL
    Uint64 lastTime; //LOCAL

    Player players [MAX_PLAYERS]; //SEE STRUCT
    SDL_FRect camera; //LOCAL

    Uint8 animationTime; //LOCAL
    bool running; //LOCAL
    bool computedEvent; //LOCAL

    //WORLD
    World world;

    // IP
    NET_Address *ipAddresses[MAX_PLAYERS];

    // UDP
    NET_DatagramSocket *udpSocket;
    NET_Datagram **udpPacket;

};

typedef struct appState *AppState;

AppState createAppState(void);

int initDisplay(AppState state);

void initCam(AppState state);