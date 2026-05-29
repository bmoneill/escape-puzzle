// game_state.c
#include "game_state.h"

void game_state_init(GameState* game) {
    map_init(&game->map);
    player_init(&game->player);
}