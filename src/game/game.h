// game.h
#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"

typedef struct {
    Map    map;
    Player player;
} GameState;

void game_init(GameState* game);
void game_exit(GameState* game);

#endif
