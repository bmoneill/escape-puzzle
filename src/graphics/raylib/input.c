/**
 * @file graphics/raylib/input.c
 * @brief Raylib implementation of input handling.
 */
#include "graphics/input.h"

#include "core/config.h"
#include "raylib.h"

// Bitfield of GAME_KEY_* values currently held down via on-screen touch controls
// (web/mobile builds only). Updated from JavaScript through touch_input_set_key()
// and merged into the keyboard state each frame in get_keys_pressed().
static u16                touch_keys_down = 0;

EMSCRIPTEN_KEEPALIVE void touch_input_set_key(int key_mask, int is_down) {
    if (is_down) {
        touch_keys_down |= (u16) key_mask;
    } else {
        touch_keys_down &= (u16) ~key_mask;
    }
}

u16 touch_input_get_keys(void) { return touch_keys_down; }

u16 get_keys_pressed(void) {
    static u16 key_map[] = { KEY_S, KEY_W, KEY_A, KEY_D, KEY_BACKSPACE, KEY_E, KEY_ESCAPE };
    u16        key_count = sizeof(key_map) / sizeof(i16);
    u16        pressed   = touch_keys_down;

    for (i16 i = 0; i < key_count; i++) {
        if (i == GAME_KEY_INTERACT || i == GAME_KEY_EXIT) {

            if (IsKeyPressed(key_map[i])) {
                pressed |= (1 << i);
            }

        } else {

            if (IsKeyDown(key_map[i])) {
                pressed |= (1 << i);
            }
        }
    }
    return pressed;
}
