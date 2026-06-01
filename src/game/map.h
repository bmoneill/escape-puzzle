/**
 * @file game/map.h
 * @brief Map header.
 */
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "tile.h"

#ifndef MAX_MAP_WIDTH
#define MAX_MAP_WIDTH 20
#endif

#ifndef MAX_MAP_HEIGHT
#define MAX_MAP_HEIGHT 15
#endif

typedef struct {
    int  width;
    int  height;

    Tile tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
} Map;

/**
 * @brief Initializes the map.
 * @param map Pointer to the Map to initialize.
 */
void map_init(Map* map);

/**
 * @brief Retrieves a pointer to the tile at the specified coordinates.
 * @param map Pointer to the Map.
 */
Tile* map_get_tile(Map* map, int x, int y);

#endif
