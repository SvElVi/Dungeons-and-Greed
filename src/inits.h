typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayID displayID;
    SDL_Rect displaySize;

    bool running;
} AppState;

int initDisplay(AppState* state);