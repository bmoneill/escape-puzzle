// input.c
#include "game_state.h"

#include "raylib.h"

void update_player(GameState* game) {
    float dx = 0;
    float dy = 0;

    if (IsKeyDown(KEY_W)) {
        dy -= game->player.move_speed;
    }

    if (IsKeyDown(KEY_S)) {
        dy += game->player.move_speed;
    }

    if (IsKeyDown(KEY_A)) {
        dx -= game->player.move_speed;
    }

    if (IsKeyDown(KEY_D)) {
        dx += game->player.move_speed;
    }

    player_move(&game->player, dx, dy);
}