#include "inits.h"

void menu_screen(Menu *menu, AppState state){
    int w, h;
    float titleX, titleY, menuY;
    float x, y;
    const float scale = 4.0f;
    const char *title = "GREEDY DELVERS";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);

    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    titleX = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) / 2;
    titleY = (h / scale) * 0.1f;    
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, titleX, titleY, title);
    menuY= (h / scale) *0.5f;
    for(int i = 0; i < menu->count; i++)
    {
        x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE* SDL_strlen(menu->menuOptions[i]))/2;
        y = menuY + i *20;

        if(i == menu->selected)
        {
            SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        }
        SDL_RenderDebugText(state->renderer, x, y, menu->menuOptions[i]);
    }
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void join_screen(AppState state){
    int w, h;
    float  x, y;
    const float scale = 4.0f;
    const char *title = "ENTER IP to join:";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);
    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    x = ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) /2;
    y = (h/ scale) /2;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, x, y, title);

    y += 40;
    x= ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE *SDL_strlen(state->hostIP)) /2;

    SDL_RenderDebugText(state->renderer, x, y, state->hostIP);
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void host_screen(AppState state){
    int w, h;
    float  x, y;
    const float scale = 4.0f;
    const char *title = "ENTER IP HOST";
    SDL_GetRenderOutputSize(state->renderer, &w, &h);
    SDL_SetRenderScale(state->renderer, scale, scale);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    x = ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(title)) /2;
    y = (h/ scale) /2;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(state->renderer, x, y, title);

    y += 40;
    x= ((w/ scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE *SDL_strlen(state->hostIP)) /2;

    SDL_RenderDebugText(state->renderer, x, y, state->hostIP);
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
}

void string_screen(AppState state, const char *inputMes){
        const char *message = inputMes;
        const char *title = "GREEDY DELVERS";
        int w = 0, h = 0;
        float titleX, titleY;
        float x, y;
        const float scale = 4.0f;

        /* Center the message and scale it up */
        SDL_GetRenderOutputSize(state->renderer, &w, &h);
        SDL_SetRenderScale(state->renderer, scale, scale);
        x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
        y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;
        titleX = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
        titleY = (h / scale) * 0.05f;

        /* Draw the message */
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->renderer);
        SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(state->renderer, x, y, message);
        SDL_RenderDebugText(state->renderer, titleX, titleY, title);
        SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);
    } 