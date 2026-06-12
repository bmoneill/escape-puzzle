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
Tile*       map_get_tile(Map* map, int x, int y);

extern Map* gmap;

#endif
