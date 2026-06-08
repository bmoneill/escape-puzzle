#ifndef PUZZLES_KEY_H
#define PUZZLES_KEY_H

#include "game/map.h"
#include "game/player.h"

typedef struct {
    i32  x;
    i32  y;
    bool collected;
} Key;

void key_spawn(Map* map, i32 x, i32 y);
void key_try_pickup(Map* map, Player* player);

#endif
