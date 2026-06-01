/**
 * @file graphics/raylib/menu.c
 * @brief Raylib implementation of the main menu.
 */
#include "graphics/menu.h"

#include "core/memory.h"

#include "raylib.h"

static bool menu_update(MenuItem*, i32, i32*);

void        menu_init(MenuItem* items, i32 num_items) {
    i32 selected = 0;
    while (menu_update(items, num_items, &selected)) {
        if (WindowShouldClose()) {
            CloseWindow();
            return;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (i32 i = 0; i < num_items; i++) {
            Color color = (i == selected) ? RED : BLACK;
            DrawText(items[i].name, 100, 100 + i * 40, 20, color);
        }

        EndDrawing();
    }
}

static bool menu_update(MenuItem* items, i32 num_items, i32* selected_item) {
    if (IsKeyPressed(KEY_UP)) {
        *selected_item = (*selected_item - 1 + num_items) % num_items;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        *selected_item = (*selected_item + 1) % num_items;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        GameState* game_state = MEM_PERM(sizeof(GameState));
        items[*selected_item].action(game_state);
        return false;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        return false;
    }
    return true;
}
