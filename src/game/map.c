/**
 * @file game/map.c
 * @brief Map implementation.
 */
#include "map.h"

#include "core/log.h"
#include "core/random.h"
#include "puzzles/lever.h"

Map*        gmap = NULL;

static void generate_rooms(Map*);
static void generate_puzzles(Map*);

void        map_init(Map* map) {
    gmap = map;
    map_generate(map);
}

void map_generate(Map* map) {
    random_seed(map->seed);

    // Fill up map with floors and bordering walls to start
    for (i32 y = 0; y < map->height; y++) {
        for (i32 x = 0; x < map->width; x++) {
            if (x == 0 || x == map->width - 1 || y == 0 || y == map->height - 1) {
                map->tiles[y][x].type  = TILE_WALL;
                map->tiles[y][x].solid = 1;

                // TODO figure out a better way to manage texture IDs
                map->tiles[y][x].texture_id = 1;

                continue;
            }
            map->tiles[y][x].type       = TILE_FLOOR;
            map->tiles[y][x].texture_id = 0;
            map->tiles[y][x].solid      = 0;
        }
    }

    // Now we determine a player start position (in top left quadrant)
    map->playerStartPos.x = random_i32_range(1, map->width / 2 - 1);
    map->playerStartPos.y = random_i32_range(1, map->height / 2 - 1);

    generate_puzzles(map);
}

Tile* map_get_tile(Map* map, int x, int y) {
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
        return 0;
    }

    return &map->tiles[y][x];
}

static void generate_walls(Map* map) {
    // TODO implement
}

static void generate_puzzles(Map* map) {
    i16 failures = 0;
    for (i16 i = 0; i < 5; i++) {

        // Try 10 times to find a random floor tile to place the lever on, otherwise just skip it
        i32 x, y;
        i8  tries = 0;
        do {
            x = random_i32_range(1, map->width - 2);
            y = random_i32_range(1, map->height - 2);
        } while ((map->tiles[y][x].type != TILE_FLOOR || x == map->playerStartPos.x
                  || y == map->playerStartPos.y)
                 && tries < 10);

        if (tries == 10) {
            failures++;
            continue;
        }

        map->tiles[y][x].type       = TILE_LEVER;
        map->tiles[y][x].texture_id = TILE_TEXTURE_LEVER_OFF;
        map->tiles[y][x].solid      = 1;

        // TODO When new puzzle types are added, this will need to be updated to randomly select a type of puzzle to place
        lever_puzzle_init(&map->puzzles[i - failures], x, y);
        map->num_puzzles++;
    }
}
