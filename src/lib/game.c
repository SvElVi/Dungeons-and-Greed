#include "game.h"

bool playersReady(AppState state)
{
    if(state->amountOfPlayers == 0)
    {
        return false;
    }
    for(int i = 0; i < state->amountOfPlayers; i++)
    {
        if(!state->players[i].classLock)
        {
            return false;
        }
    }
    return true;
}

void updateLobby(AppState state)
{
    if(state->gameState != GAME_LOBBY)
    {
        return;
    }
    if(playersReady(state))
    {
        state->gameState = GAME_PLAYING;
    }
}