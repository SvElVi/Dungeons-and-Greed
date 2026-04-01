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

void moveFlag(Player_Flags* flags, const bool* keylist) {
    if(keylist[SDL_SCANCODE_W] && !keylist[SDL_SCANCODE_S]) {
        flags->moveY = -1;
    } else if(keylist[SDL_SCANCODE_S] && !keylist[SDL_SCANCODE_W]) {
        flags->moveY = 1;
    } else {
        flags->moveY = 0;
    }

    if(keylist[SDL_SCANCODE_A] && !keylist[SDL_SCANCODE_D]) {
        flags->moveX = -1;
    } else if(keylist[SDL_SCANCODE_D] && !keylist[SDL_SCANCODE_A]) {
        flags->moveX = 1;
        
    } else {
        flags->moveX = 0;
    }
}

int checkEvents(AppState* state, SDL_Event* event) {

    const bool* keylist = SDL_GetKeyboardState(0);

    int quitEvent = QuitEvent(state, event, keylist);
    if(quitEvent) return quitEvent;

    //Non quit functions
    moveFlag(&(state->players[0].flags), keylist);
    //if frame altering function then set state->computedEvent = true and add to the if statement in the render() function

    return SDL_APP_CONTINUE;
}