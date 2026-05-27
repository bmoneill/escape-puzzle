/**
 * @brief User interface management header.
 */
#ifndef UI_H
#define UI_H

#include "raylib.h"

typedef enum { UI_STATE_MAIN_MENU, UI_STATE_IN_GAME, UI_STATE_PAUSE, UI_STATE_GAME_OVER } UIState;

typedef struct {
    UIState state;
    int     selected_menu_item;
} UI;

void ui_init(UI* ui);

#endif
