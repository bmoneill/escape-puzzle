#include "game/mapgen.h"
#include "core/random.h"
#include "game/tile.h"

static void generate_rooms(Map*);
static void generate_puzzles(Map*);

void        generate_map(Map* map, MapParams* params) {
    random_seed(params->seed);

    map->width  = params->width;
    map->height = params->height;

    // Fill up the map with floors to start
    for (i32 y = 0; y < params->height; y++) {
        for (i32 x = 0; x < params->width; x++) {
            map->tiles[y][x].type       = TILE_FLOOR;
            map->tiles[y][x].texture_id = 0;
            map->tiles[y][x].solid      = 0;
        }
    }

    // Now we determine a player start position and exit position
    // before generating everything else

    // Generate all the rooms and stuff first, then add puzzles on top of that
}

static void generate_walls(Map* map) {
    // TODO implement
}

static void generate_puzzles(Map* map) {
    // TODO implement
}
