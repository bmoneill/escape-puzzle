// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "core/types.h"

#ifndef DEFAULT_PLAYER_SPEED
#define DEFAULT_PLAYER_SPEED 1.0
#endif

typedef struct {
    f64 x;
    f64 y;

    f64 move_speed;

    i32 width;
    i32 height;
} Player;

void player_init(Player* player);
void player_move(Player* player, i16 keys);
void player_update(Player* player);

#endif
