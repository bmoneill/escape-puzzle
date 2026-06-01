/**
 * @file graphics/raylib/render.c
 * @brief Raylib implementation of the rendering system.
 */
#include "graphics/render.h"

#include "game/game.h"

#include "raylib.h"

void render_init(void) {
    InitWindow(800, 600, "Escape Puzzle");
    SetTargetFPS(60);
}

void render_frame(GameState* game) {
    if (WindowShouldClose()) {
        CloseWindow();
        game_exit(game);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Player placeholder
    DrawRectangle(game->player.x, game->player.y, game->player.width, game->player.height, BLUE);

    EndDrawing();
}
