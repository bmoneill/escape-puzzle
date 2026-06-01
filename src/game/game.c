// game.c
#include "game.h"
#include "graphics/render.h"

#include <stdlib.h>

void game_init(GameState* game) {
    map_init(&game->map);
    player_init(&game->player);

    while (1) {
        player_update(&game->player);
        render_frame(game);

        // TODO Render the game state here
    }
}

void game_exit(GameState* game) {
    // TODO Perform any necessary cleanup here
    exit(0);
}
