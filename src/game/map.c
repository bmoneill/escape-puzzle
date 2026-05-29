// map.c
#include "map.h"

void map_init(Map* map) {

    map->width  = MAP_WIDTH;
    map->height = MAP_HEIGHT;

    for (int y = 0; y < MAP_HEIGHT; y++) {

        for (int x = 0; x < MAP_WIDTH; x++) {

            Tile* tile = &map->tiles[y][x];

            // Default tile
            tile->type       = TILE_FLOOR;
            tile->texture_id = 0;
            tile->solid      = 0;

            // Border walls
            if (
                x == 0 ||
                y == 0 ||
                x == MAP_WIDTH - 1 ||
                y == MAP_HEIGHT - 1
            ) {
                tile->type       = TILE_WALL;
                tile->texture_id = 1;
                tile->solid      = 1;
            }
        }
    }
}

Tile* map_get_tile(
    Map* map,
    int x,
    int y
) {

    // Out of bounds check
    if (
        x < 0 ||
        y < 0 ||
        x >= map->width ||
        y >= map->height
    ) {
        return 0;
    }

    return &map->tiles[y][x];
}