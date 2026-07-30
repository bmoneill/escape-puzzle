/**
 * @file graphics/input.h
 * @brief Graphics library-agnostic input handling.
 */
#ifndef GRAPHICS_INPUT_H
#define GRAPHICS_INPUT_H

#include "core/types.h"

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

/**
 * @brief Sets or clears a virtual key state driven by on-screen touch controls (used by the web build's mobile UI).
 *
 * The result is merged into get_keys_pressed() every frame, alongside physical keyboard input. Intended to be
 * called from JavaScript (via the exported symbol) when a touch control button is pressed or released.
 *
 * @param key_mask One of the GAME_KEY_* bit values.
 * @param is_down Non-zero if the virtual button is currently pressed, zero if released.
 */
void touch_input_set_key(int key_mask, int is_down);

/**
 * @brief Returns the current bitfield of GAME_KEY_* values held down via on-screen touch controls.
 *
 * Unlike get_keys_pressed(), this reports only the touch button state (no keyboard), so callers outside the
 * main gameplay loop (e.g. menu navigation) can apply their own edge-detection semantics on top of it.
 *
 * @return A bitfield of GAME_KEY_* values currently held down via touch_input_set_key().
 */
u16 touch_input_get_keys(void);

#endif
