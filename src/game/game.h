/**
 * @file game/game.h
 * @brief Core game logic header.
 */
#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"

/**
 * @brief Represents the entire state of the game, including the map and player.
 */
typedef struct {
    Map    map; //!< The current game map, including layout and objects.
    Player player; //!< The player character, including position and state.
} GameState;

/**
 * @brief Initializes the game state and enters the game loop
 *
 * @param game Pointer to the GameState to initialize.
 */
void game_init(GameState* game);

/**
 * @brief Exits the game, performing any necessary cleanup.
 * @param game Pointer to the GameState.
 */
void game_exit(GameState* game);

#endif
