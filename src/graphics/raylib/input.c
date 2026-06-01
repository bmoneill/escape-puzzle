/**
 * @file graphics/raylib/input.c
 * @brief Raylib implementation of input handling.
 */
#include "../input.h"
#include "game/game.h"

#include "raylib.h"

i16 get_keys_pressed(void) {
    static i16 key_map[] = { KEY_S, KEY_W, KEY_A, KEY_D, KEY_BACKSPACE, KEY_E, KEY_ESCAPE };
    i16        key_count = sizeof(key_map) / sizeof(i16);
    i64        pressed   = 0;

    for (i16 i = 0; i < key_count; i++) {
        if (IsKeyDown(key_map[i])) {
            pressed |= (1 << i);
        }
    }

    return pressed;
}
