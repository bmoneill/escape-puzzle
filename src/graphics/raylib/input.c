/**
 * @file graphics/raylib/input.c
 * @brief Raylib implementation of input handling.
 */
#include "graphics/input.h"

#include "raylib.h"

u16 get_keys_pressed(void) {
    static u16 key_map[] = { KEY_S, KEY_W, KEY_A, KEY_D, KEY_BACKSPACE, KEY_E, KEY_ESCAPE };
    u16        key_count = sizeof(key_map) / sizeof(i16);
    u16        pressed   = 0;

    for (i16 i = 0; i < key_count; i++) {
        if (IsKeyDown(key_map[i])) {
            pressed |= (1 << i);
        }
    }

    return pressed;
}
