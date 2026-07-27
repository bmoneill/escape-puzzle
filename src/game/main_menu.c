/**
 * @file game/main_menu.c
 * @brief Main menu implementation.
 */
#include "main_menu.h"

#include "core/config.h"
#include "game/game.h"
#include "graphics/menu.h"

#include <stdlib.h>

EMSCRIPTEN_KEEPALIVE void main_menu_init(void) {
    menu_init(
        (MenuItem[]){
            { "Start Game", game_init },
            { "Settings", game_exit },
            { "Exit", game_exit },
        },
        3);
}
