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

ClearBackground((Color){18, 18, 30, 255});

// Title
DrawText("PUZZLE REALM",
         GetScreenWidth()/2 - MeasureText("PUZZLE REALM", 48)/2,
         50,
         48,
         GOLD);

DrawText("Solve the puzzles. Escape the realm.",
         GetScreenWidth()/2 - MeasureText("Solve the puzzles. Escape the realm.", 20)/2,
         105,
         20,
         LIGHTGRAY);

// Menu box
int boxX = GetScreenWidth()/2 - 220;
int boxY = 170;
int boxW = 440;
int boxH = num_items * 55 + 30;

DrawRectangle(boxX, boxY, boxW, boxH, (Color){35,35,45,255});
DrawRectangleLines(boxX, boxY, boxW, boxH, GRAY);

// Menu items
for (i32 i = 0; i < num_items; i++) {

    int y = boxY + 20 + i * 55;

    if (i == selected) {

        DrawRectangle(boxX + 10, y - 5, boxW - 20, 40, DARKBLUE);

        DrawText(">",
                 boxX + 25,
                 y,
                 28,
                 YELLOW);

        DrawText(items[i].name,
                 boxX + 60,
                 y,
                 28,
                 YELLOW);

    } else {

        DrawText(items[i].name,
                 boxX + 60,
                 y,
                 28,
                 RAYWHITE);
    }
}

// Controls
DrawText("UP/DOWN  Navigate",
         GetScreenWidth()/2 - MeasureText("UP/DOWN  Navigate",18)/2,
         GetScreenHeight()-80,
         18,
         LIGHTGRAY);

DrawText("ENTER  Select    ESC  Quit",
         GetScreenWidth()/2 - MeasureText("ENTER  Select    ESC  Quit",18)/2,
         GetScreenHeight()-50,
         18,
         LIGHTGRAY);

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
