#ifndef GRAPHICS_MENU_H
#define GRAPHICS_MENU_H

#include "core/types.h"
#include "game/game.h"

typedef struct {
    const char* name;
    void (*action)(GameState* game);
} MenuItem;

typedef struct {
    MenuItem* items;
    i32       num_items;
    i32       selected_menu_item;
} Menu;

void menu_init(MenuItem* items, i32 num_items);

#endif // GRAPHICS_MENU_H
