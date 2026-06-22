/**
 * @file game/map.h
 * @brief Map header.
 */
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "game/tile.h"
#include "puzzles/puzzle.h"

#ifndef MAX_MAP_WIDTH
#define MAX_MAP_WIDTH 20
#endif

#ifndef MAX_MAP_HEIGHT
#define MAX_MAP_HEIGHT 15
#endif

typedef enum {
    THEME_NONE,
    THEME_DUNGEON,
    THEME_TEMPLE,
    THEME_PYRAMID,
} MapTheme;

typedef struct {
    i32      width;
    i32      height;
    vec2f    playerStartPos;
    i64      seed;
    f32      difficulty; //!< Unused for now
    MapTheme theme; //!< Unused for now

    Tile     tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
    Puzzle   puzzles[MAX_PUZZLES];
    i32      num_puzzles;
    i32      currentLeverOrder;
} Map;

/**
 * @brief Initializes the map.
 * @param map Pointer to the Map to initialize.
 */
void map_init(Map* map);

/**
 * @brief Generates the map layout, including rooms, walls, and puzzles.
 * @param map Pointer to the Map to generate.
 */
void map_generate(Map* map);

/**
 * @brief Retrieves a pointer to the tile at the specified coordinates.
 * @param map Pointer to the Map.
 */
Tile* map_get_tile(Map* map, int x, int y);

/**
 * @brief Returns true if all lever puzzles have been activated.
 * @param map Pointer to the Map.
 */
bool map_all_levers_active(Map* map);

/**
 * @brief Returns true if tile at coords is adjacent to a tile of type type
 * @param map map to check
 * @param x x coord of tile to check
 * @param y y coord of tile to check
 * @param type Tile type to check if (x,y) is adjacent to it
 */
bool        tile_adjacent_to(Map* map, int x, int y, TileType type);

extern Map* gmap;

#endif
