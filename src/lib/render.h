#ifndef RENDER_H
#define RENDER_H

#include "events.h"


int renderGamePlay(AppState state);
void menu_screen(Menu *menu, AppState state);
void join_screen(AppState state);
void host_screen(AppState state);
void pause_screen(AppState state);
int render(AppState state);

#endif