#include <SDL3/SDL.h>
#include "inits.h"

int QuitEvent(AppState* state, SDL_Event* event, const bool* keylist) {
    if (keylist[SDL_SCANCODE_ESCAPE] ||
        event->type == SDL_EVENT_QUIT) {
        state->running = false;
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

int checkEvents(AppState* state, SDL_Event* event) {

    const bool* keylist = SDL_GetKeyboardState(0);

    int quitEvent = QuitEvent(state, event, keylist);
    if(quitEvent) return quitEvent;

    return SDL_APP_CONTINUE;
}