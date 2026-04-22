#include <SDL3/SDL.h>
#include "inits.h"
#include "render.h"
#include "menu.h"

int QuitEvent(AppState state, SDL_Event* event, const bool* keylist) {
    if (event->type == SDL_EVENT_QUIT) {
        state->running = false;
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void moveFlag(Player_Flags* flags, const bool* keylist, bool* flag) { //Flag for player movement
    if((keylist[SDL_SCANCODE_W] || keylist[SDL_SCANCODE_UP]) && !(keylist[SDL_SCANCODE_S] || keylist[SDL_SCANCODE_DOWN])) {
        flags->moveY = -1;
    } else if((keylist[SDL_SCANCODE_S] || keylist[SDL_SCANCODE_DOWN]) && !(keylist[SDL_SCANCODE_W] || keylist[SDL_SCANCODE_UP])) {
        flags->moveY = 1;
    } else {
        flags->moveY = 0;
    }

    if((keylist[SDL_SCANCODE_A] || keylist[SDL_SCANCODE_LEFT]) && !(keylist[SDL_SCANCODE_D] || keylist[SDL_SCANCODE_RIGHT])) {
        flags->moveX = -1;
    } else if((keylist[SDL_SCANCODE_D] || keylist[SDL_SCANCODE_RIGHT]) && !(keylist[SDL_SCANCODE_A] || keylist[SDL_SCANCODE_LEFT])) {
        flags->moveX = 1;
    } else {
        flags->moveX = 0;
    }

    *flag = true;
}

int checkEvents(AppState state, SDL_Event* event) { //Check all in game events used

    const bool* keylist = SDL_GetKeyboardState(0);

    int quitEvent = QuitEvent(state, event, keylist);
    if(quitEvent) return quitEvent;

    if(state->gameState ==GAME_MENY){
        static bool upLast = false;
        static bool downLast = false;
        static bool enterLast = false;

        if((keylist[SDL_SCANCODE_UP] || keylist[SDL_SCANCODE_W]) && !upLast){
            state->mainMenu.selected--;
            if(state->mainMenu.selected < 0) state->mainMenu.selected = state->mainMenu.count -1;
        }
        if((keylist[SDL_SCANCODE_DOWN] || keylist[SDL_SCANCODE_S]) && !downLast){
            state->mainMenu.selected++;
            if(state->mainMenu.selected >= state->mainMenu.count) state->mainMenu.selected = 0;
        }
        if(keylist[SDL_SCANCODE_RETURN] && !enterLast){
            switch(state->mainMenu.selected){
                case 0:
                    state->gameState =GAME_JOIN;
                    state->hostIPLen = 0;
                    state->hostIP[0] = '\0';
                    SDL_StartTextInput(state->window);
                break;
                case 1:
                    state->gameState = GAME_PLAYING;
                break;
                case 2:
                    state->running = false;
                    return SDL_APP_SUCCESS;
                break;
            }
        }
        upLast = keylist[SDL_SCANCODE_UP];
        downLast = keylist[SDL_SCANCODE_DOWN];
        enterLast = keylist[SDL_SCANCODE_RETURN];
    }
    if(state->gameState == GAME_HOST || state->gameState == GAME_JOIN){

        if(event->type == SDL_EVENT_KEY_DOWN){
            if(event->key.key == SDLK_BACKSPACE){
                if(state->hostIPLen > 0){
                    state->hostIPLen--;
                    state->hostIP[state->hostIPLen]='\0';
                }
            }
            else if(event->key.key == SDLK_RETURN && state->hostIPLen > 0){
                SDL_StopTextInput(state->window);
                SDL_Log("Connecting/hosting with IP: %s", state->hostIP);
                state->gameState = GAME_TCP_INIT;
            }
            else if(event->key.key ==SDLK_ESCAPE){
                state->gameState = GAME_MENY;
            }
            else{
                char c = 0;
                if(event->key.key >= SDLK_0 && event->key.key <= SDLK_9){
                    c = '0' + (event->key.key - SDLK_0);
                }
                else if(event->key.key == SDLK_PERIOD){
                    c = '.';
                }
                if(c != 0 && state->hostIPLen <16){
                    state->hostIP[state->hostIPLen++] = c;
                    state->hostIP[state->hostIPLen] = '\0';
                }
            }
        }
    }
    //Non quit functions
    static bool escLast = false;
    if(state->gameState == GAME_PLAYING){
        moveFlag(&(state->players[0].flags), keylist, &(state->computedEvent));

        if(keylist[SDL_SCANCODE_ESCAPE] && !escLast) {
            state->gameState = GAME_PAUSE;
        }

    }
    else if(state->gameState == GAME_PAUSE){
        if(keylist[SDL_SCANCODE_ESCAPE] && !escLast) {
            state->running = false;
            return SDL_APP_SUCCESS;

        }
        else if(keylist[SDL_SCANCODE_SPACE]){
            state->gameState = GAME_PLAYING;
        }
    }
    escLast = keylist[SDL_SCANCODE_ESCAPE];

    //if frame altering function then set state->computedEvent = true and add to the if statement in the render() function



    return SDL_APP_CONTINUE;
}