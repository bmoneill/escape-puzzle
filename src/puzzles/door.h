#ifndef PUZZLES_DOOR_H
#define PUZZLES_DOOR_H

#include "game/map.h"
#include "game/player.h"

typedef struct {
    i32 x;
    i32 y;
    bool locked;
} Door;

void door_spawn(Map* map, i32 x, i32 y);
void door_try_open(Map* map, Player* player);

#endif