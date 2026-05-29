// player.c
#include "player.h"

void player_init(Player* player) {
    player->x = 64;
    player->y = 64;

    player->move_speed = 4.0f;

    player->width  = 32;
    player->height = 32;
}

void player_move(Player* player, float dx, float dy) {
    player->x += dx;
    player->y += dy;
}