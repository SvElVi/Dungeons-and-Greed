#ifndef MENU_H
#define MENU_H
#include "inits.h"


void menu_screen(Menu *menu, AppState state);
void join_screen(AppState state);
void host_screen(AppState state);
void string_screen(AppState state, const char *inputMes);
const char* getClass(Player_Class c);
#endif