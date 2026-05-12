#include <string.h>
#include <stdio.h>

#include "player.h"
#include "enemy.h"
#include "render.h"
#include "hud.h"
#include "menu.h"

int renderFrame(AppState state, Player *player)
{

    switch (state->gameState)
    {
    case SERVER:
        string_screen(state, "SERVER!");
        break;

    case GAME_MENY:
        menu_screen(&state->mainMenu, state);
        break;

    case GAME_PAUSE:
        string_screen(state, "Press SPACE to continue or ESC to quit!");
        break;

    case GAME_JOIN:
        join_screen(state);
        break;

    case GAME_LOBBY:
        lobby_screen(state);
        break;

    case GAME_TCP_INIT:
        string_screen(state, "Connecting...");
        break;

    case GAME_HANDSHAKE:
        string_screen(state, "Handshake...");
        break;

    case GAME_VERIFYING_HANDSHAKE:
        string_screen(state, "Verifying handshake...");
        break;

    case GAME_WAITING_FOR_OTHER_PLAYERS:
        char tempStr[64];
        snprintf(tempStr, 64, "Waiting for other players... %d of %d connected!", state->connectedPlayers.amountOfPlayers, MAX_PLAYERS);
        string_screen(state, tempStr);
        break;

    case GAME_PLAYING:
        renderGamePlay(state, player);
        break;

    case GAME_SERVER_SHUTDOWN:
        string_screen(state, "Server shutdown! Press SPACE to continue!");
        break;

    case GAME_DEAD:
        string_screen(state, "DEAD!");
        break;
    }

    SDL_RenderPresent(state->renderer);
    state->computedEvent = false;

    return SDL_APP_CONTINUE;
}

int render(AppState state, Player *player)
{
    Uint64 currentTime = SDL_GetTicks();
    if (currentTime >= state->lastTime + SDL_round(1000 / state->framerate))
    {
        state->deltaTime = currentTime - state->lastTime;
        state->lastTime = currentTime;
        if (state->gameState == GAME_PLAYING)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                movement(&(state->players[i]), state->players, state->enemies, state->deltaTime, state->world);
            }
            animatePlayers(state->players, &(state->animationTime), state->framerate, &(state->computedEvent));

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (state->enemies[i].state != ENEMY_DEAD)
                    enemyMovement(&state->enemies[i], state->players, state->deltaTime, state->world);
            }
            playerEnemyCollision(state->curPlayerPtr, state->enemies, state->deltaTime);
            animateEnemies(state->enemies, &state->enemyAnimationTime, state->framerate, &state->computedEvent);
        }

        if (renderFrame(state, player))
            return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

int renderGamePlay(AppState state, Player *player)
{
    SDL_FRect temp;
    Vector2D tempV, renderOrder[MAX_PLAYERS + MAX_ENEMIES];
    int renderCount = 0;
    SDL_SetRenderDrawColor(state->renderer, 37, 19, 26, 1);
    SDL_RenderClear(state->renderer);

    if (!renderDungeon(state, player))
    {
        return SDL_APP_FAILURE;
    }

    for (int j = 0; j < MAX_PLAYERS; j++)
    {
        renderOrder[j].x = j;
        renderOrder[j].y = state->players[j].pos.y;
        renderCount++;
    }

    for (int j = 0; j < MAX_ENEMIES; j++)
    {
        if (state->enemies[j].texture && state->enemies[j].state != ENEMY_DEAD)
        {
            renderOrder[renderCount].x = MAX_PLAYERS + j;
            renderOrder[renderCount].y = state->enemies[j].pos.y;
            renderCount++;
        }
    }

    for (int a = 0; a < renderCount - 1; a++)
    {
        for (int b = 0; b < renderCount - 1 - a; b++)
        {
            if (renderOrder[b].y < renderOrder[b + 1].y)
            {
                tempV = renderOrder[b];
                renderOrder[b] = renderOrder[b + 1];
                renderOrder[b + 1] = tempV;
            }
        }
    }

    for (int i = 0; i < renderCount; i++)
    {
        if (renderOrder[i].x < MAX_PLAYERS)
        {
            if (!state->players[renderOrder[i].x].texture)
                continue;

            temp.h = PLAYER_SIZE * RENDER_SCALE;
            temp.w = PLAYER_SIZE * RENDER_SCALE;
            temp.x = state->camera.x + (player->pos.x - state->players[renderOrder[i].x].pos.x);
            temp.y = state->camera.y + (player->pos.y - state->players[renderOrder[i].x].pos.y);

            if (!(SDL_RenderTextureRotated(state->renderer, state->players[renderOrder[i].x].texture, &(state->players[renderOrder[i].x].aniBox), &(temp), 0, NULL, state->players[renderOrder[i].x].flip)))
            {
                SDL_Log("FAILED RENDERING TEXTURE: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            if (!drawHpBarAbove(state->renderer, &state->players[renderOrder[i].x].stats, &temp))
            {
                return SDL_APP_FAILURE;
            }

            if (!drawPlayerNameBelow(state->renderer, &state->players[renderOrder[i].x], &temp))
            {
                return SDL_APP_FAILURE;
            }
        }
        else
        {
            SDL_FRect dst = {
                .w = ENEMY_SPRITE_SIZE * RENDER_SCALE,
                .h = ENEMY_SPRITE_SIZE * RENDER_SCALE,
                .x = state->camera.x + (player->pos.x - state->enemies[renderOrder[i].x - MAX_PLAYERS].pos.x),
                .y = state->camera.y + (player->pos.y - state->enemies[renderOrder[i].x - MAX_PLAYERS].pos.y)};

            SDL_RenderTextureRotated(state->renderer, state->enemies[renderOrder[i].x - MAX_PLAYERS].texture,
                                     &state->enemies[renderOrder[i].x - MAX_PLAYERS].aniBox, &dst, 0, NULL, state->enemies[renderOrder[i].x - MAX_PLAYERS].flip);

            if (!drawHpBarAbove(state->renderer, &state->enemies[renderOrder[i].x - MAX_PLAYERS].stats, &dst))
            {
                return SDL_APP_FAILURE;
            }
        }
    }

    return SDL_APP_CONTINUE;
}