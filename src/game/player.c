// player.c
#include "player.h"

#include "graphics/input.h"

#include <stdio.h>
#include <stdlib.h>

void player_init(Player* player) {
    player->x          = 64;
    player->y          = 64;

    player->move_speed = DEFAULT_PLAYER_SPEED;

    player->width      = 32;
    player->height     = 32;
}

void player_update(Player* player) {
    i16 keys_pressed = get_keys_pressed();
    if (keys_pressed & GAME_KEY_EXIT) {
        // TODO Handle any necessary cleanup here
        exit(0);
    }
    player_move(player, keys_pressed);
}

void player_move(Player* player, i16 keys) {
    float dx = 0;
    float dy = 0;

    if (keys & GAME_KEY_UP) {
        dy -= player->move_speed;
    }
    if (keys & GAME_KEY_DOWN) {
        dy += player->move_speed;
    }
    if (keys & GAME_KEY_LEFT) {
        dx -= player->move_speed;
    }
    if (keys & GAME_KEY_RIGHT) {
        dx += player->move_speed;
    }

    player->x += dx;
    player->y += dy;
}
