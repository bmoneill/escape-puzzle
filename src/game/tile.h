/**
 * @file game/tile.h
 * @brief Tile header.
 */
#ifndef GAME_TILE_H
#define GAME_TILE_H

#include "core/types.h"

/**
 * @brief Enumeration of different tile types in the game.
 */
typedef enum { TILE_FLOOR, TILE_WALL, TILE_LEVER, TILE_EXIT } TileType;

/**
 * @brief Represents a single tile in the game map, including its type, texture, and solidity.
 */
typedef struct {
    TileType type; //!< The type of the tile (e.g., floor, wall, lever, exit).
    u32      texture_id; //!< The ID of the texture to use when rendering this tile.
    u32 solid; //!< Whether the tile is solid (1) or passable (0), affecting player movement and interactions.
} Tile;

#endif
