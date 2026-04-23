#ifndef INITS_H
#define INITS_H

#include <SDL3_net/SDL_net.h>

#define MAX_PLAYERS 5
#define PLAYER_SIZE 48
#define RENDER_SCALE 4
#define ANIMATION_TIME 4
#define PLAYER_NAME_MAX 30

#define MAX_ENEMIES 1
#define ENEMY_SIZE 48
#define ENEMY_AGGRO_RANGE 150
#define ENEMY_ATTACK_RANGE 20

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
    ASSIGNING_PLAYER_ID,
    SENDING_PLAYER_ID,
    CONFIRMING_PLAYER_ID_RECIVE,
    STARTING_GAME,
    GAME_ONGOING,
    SERVER_CLEANUP
} ServerState;

typedef struct world *World;

typedef struct{
    const char *menuOptions[3];
    int selected;
    int count;
} Menu;

typedef struct {
    int health;
    int maxHealth;
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
    char name[PLAYER_NAME_MAX]; // player name string
    Uint32 enemyCollisionTimer; // ms counter for character colliding with emeny. SYNC MULTIPLAYER
} Player;

typedef enum {
    ENEMY_SKELETON,
} Enemy_Type;

typedef enum {
    ENEMY_IDLE,
    ENEMY_CHASE,
    ENEMY_ATTACK,
    ENEMY_DEAD
} Enemy_State;

typedef struct {
    Vector2D pos;
    SDL_FRect hitBox;
    SDL_FRect aniBox;
    SDL_Texture* texture;
    SDL_FlipMode flip;
    direction facing;

    Enemy_Type type;
    Enemy_State state;
    Stats stats;

    int moveX;
    int moveY;
} Enemy;

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
    GAME_MENY,
    GAME_HOST,
    GAME_JOIN,
    GAME_INIT,
    GAME_TCP_INIT,
    GAME_TCP_HANDSHAKE,
    GAME_TCP_VERIFYING_HANDSHAKE,
    GAME_TCP_HANDSHAKE_SUCCESFULL,
    GAME_TCP_HANDSHAKE_FAILURE,
    GAME_START,
    GAME_PLAYING,
    GAME_PAUSE,
    GAME_OVER,
    SERVER
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
    Menu mainMenu;

    Player players [MAX_PLAYERS]; //SEE STRUCT
    SDL_FRect camera; //LOCAL

    Enemy enemies [MAX_ENEMIES];

    Uint8 animationTime; //LOCAL
    Uint8 enemyAnimationTime; //LOCAL
    bool running; //LOCAL
    bool computedEvent; //LOCAL

    //WORLD
    World world; //LOCAL
    ConnectedPlayers connectedPlayers;

    // Server IP
    NET_Address *serverIP;
    char hostIP[16];
    int hostIPLen;
    // UDP
    NET_DatagramSocket *udpSocket;
    NET_Datagram **udpPacket;

    // Server TCP
    NET_Server *tcpServer;
    NET_StreamSocket *serverStreamSocket;

    // Client TCP
    NET_StreamSocket *tcpClient;

};

typedef struct appState *AppState;

AppState createAppState(void);

int initDisplay(AppState state);

void initCam(AppState state);

#endif