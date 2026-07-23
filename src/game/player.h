/**
 * @file game/player.h
 * @brief Player header.
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "core/types.h"
#include "map.h"

typedef enum {
    PLAYER_DIR_DOWN,
    PLAYER_DIR_UP,
    PLAYER_DIR_LEFT,
    PLAYER_DIR_RIGHT,
} PlayerDirection;

#ifndef DEFAULT_PLAYER_SPEED
#define DEFAULT_PLAYER_SPEED 3.0
#endif

#define PLAYER_TILE_COORD(player)                                                                  \
    (vec2u) { .x = (i32) ((player)->x / TILE_SIZE), .y = (i32) ((player)->y / TILE_SIZE) }

typedef struct {
    f64 x; //!< Player's x-coordinate.
    f64 y; //!< Player's y-coordinate.

    f64 move_speed; //!< Player's movement speed in pixels per frame.

    u16 width; //!< Player's width in pixels.
    u16 height; //!< Player's height in pixels.

    i32 keys; // inventory

    PlayerDirection facing;   //!< Direction the player is currently facing.
    bool            is_moving; //!< Whether the player moved this frame.
    u32             anim_tick; //!< Increments each frame while moving; resets on stop.
} Player;

/**
 * @brief Initializes the player with default values.
 * @param player Pointer to the Player to initialize.
 */
void player_init(Player* player);

/**
 * @brief Moves the player based on the current key inputs.
 * @param player Pointer to the Player to move.
 */
void player_move(Player* player, Map* map, i16 keys);

/**
 * @brief Updates the player's state, including movement and interactions.
 * @param player Pointer to the Player to update.
 * @param map Pointer to the Map.
 * @param keys_pressed The current key inputs for player movement and actions.
 */
void player_update(Player* player, Map* map, i16 keys_pressed);

#endif
