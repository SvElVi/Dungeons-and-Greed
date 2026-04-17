#ifndef INITS_H
#define INITS_H

#include <SDL3_net/SDL_net.h>

#define MAX_PLAYERS 5
#define PLAYER_SIZE 48
#define RENDER_SCALE 2
#define ANIMATION_TIME 4

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

// Serverside state
typedef enum {
    INIT_OF_SERVER,
    WAITING_FOR_PLAYERS,
    STARTING_GAME,
    GAME_ONGOING,
    SERVER_CLEANUP
} ServerState;

typedef struct world *World;

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
    SDL_FRect hitBox;
    SDL_FRect aniBox; //LOCAL
    direction facing; //SYNC MULTIPLAYER
    SDL_FlipMode flip; //SYNC MULTIPLAYER
    NET_StreamSocket **playerStreamSocket;
} Player;

// Serverside player struct
typedef struct {
    Vector2D pos;
    Player_Flags flags;
    Player_Class class;
    Stats stats;
    SDL_FRect hitBox;
    direction facing;
    SDL_FlipMode flip;
    NET_StreamSocket **playerStreamSocket;
} ServerPlayer;

// Serverside players
typedef struct {
    int amountOfPlayers;
    ServerPlayer players[MAX_PLAYERS];

} ConnectedPlayers;

typedef enum GameState {
    //GAME_HOST
    //GAME_JOIN
    GAME_START,
    GAME_PLAYING,
    GAME_PAUSE,
    GAME_OVER   
} GameState;

struct appState {
    SDL_Window *window; //LOCAL
    SDL_Renderer *renderer; //LOCAL
    SDL_DisplayID displayID; //LOCAL
    const SDL_DisplayMode* displayMode; //LOCAL
    float framerate; //LOCAL
    Uint64 deltaTime; //LOCAL
    Uint64 lastTime; //LOCAL
    ServerState serverState;
    GameState gameState;

    Player players [MAX_PLAYERS]; //SEE STRUCT
    SDL_FRect camera; //LOCAL

    Uint8 animationTime; //LOCAL
    bool running; //LOCAL
    bool computedEvent; //LOCAL

    //WORLD
    World world; //LOCAL

    ConnectedPlayers connectedPlayers;

    // Server IP
    NET_Address *serverIP;

    // UDP
    NET_DatagramSocket *udpSocket;
    NET_Datagram **udpPacket;

    // Server TCP
    NET_Server *tcpServer;

    // Client TCP
    NET_StreamSocket *tcpClient;

};

typedef struct appState *AppState;

AppState createAppState(void);

int initDisplay(AppState state);

void initCam(AppState state);

#endif