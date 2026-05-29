// game_state.h
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "map.h"
#include "player.h"

typedef struct {
    Map map;
    Player player;
} GameState;

void game_state_init(GameState* game);

#endif