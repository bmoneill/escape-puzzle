/**
 * @file puzzles/cipher.h
 * @brief Header for the substitution cipher door puzzle.
 *
 * A random substitution cipher is generated each time the player interacts
 * with a cipher door.  The full 26-letter encode key is displayed alongside
 * the encoded word; the player must reverse it and type the plaintext.
 */
#ifndef PUZZLES_CIPHER_H
#define PUZZLES_CIPHER_H

#include "game/map.h"

/**
 * @brief Spawns a cipher door tile at (x, y) on the map.
 * @param map Pointer to the Map.
 * @param x   Tile x-coordinate.
 * @param y   Tile y-coordinate.
 */
void cipher_door_spawn(Map* map, i32 x, i32 y);

/**
 * @brief Runs the cipher puzzle for the given tile.
 *        Unlocks (converts to floor) the tile on a correct answer.
 * @param tile Pointer to the cipher door tile being interacted with.
 */
void cipher_door_try(Tile* tile);

#endif
