/**
 * @file graphics/input.h
 * @brief Graphics library-agnostic input handling.
 */
#ifndef GRAPHICS_INPUT_H
#define GRAPHICS_INPUT_H

#include "core/types.h"
#include "game/game.h"

// Global key state bitfield. Each bit represents a key being pressed.
#define GAME_KEY_DOWN     1
#define GAME_KEY_UP       2
#define GAME_KEY_LEFT     4
#define GAME_KEY_RIGHT    8
#define GAME_KEY_BACK     16
#define GAME_KEY_INTERACT 32
#define GAME_KEY_EXIT     64

/**
 * @brief Retrieves the current state of key presses as a bitfield, where each bit corresponds to a specific key defined by the GAME_KEY_* constants.
 * @return A 16-bit unsigned integer representing the current key states, with bits set accordingly.
 */
u16 get_keys_pressed(void);

#endif
