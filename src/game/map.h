// map.h
#ifndef MAP_H
#define MAP_H

#include "tile.h"

#define MAP_WIDTH  20
#define MAP_HEIGHT 15

typedef struct {
    int  width;
    int  height;

    Tile tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void  map_init(Map* map);

Tile* map_get_tile(Map* map, int x, int y);

#endif
