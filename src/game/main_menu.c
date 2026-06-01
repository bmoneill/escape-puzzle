#include "main_menu.h"

#include "core/config.h"

#include "graphics/menu.h"

#include <stdlib.h>

void exit_game(GameState* game);

void main_menu_init(void) {
    menu_init(
        (MenuItem[]) {
            { "Start Game", game_init },
            { "Settings", exit_game },
            { "Exit", exit_game },
        },
        3);
}

void exit_game(GameState* game) {
    // TODO Perform any necessary cleanup here
    exit(0);
}
