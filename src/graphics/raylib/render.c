/**
 * @file graphics/raylib/render.c
 * @brief Raylib implementation of the rendering system.
 */
#include "graphics/render.h"

#include "game/game.h"
#include "game/map.h"
#include "game/tile.h"

#include "raylib.h"

#define TILE_SIZE 32

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

    /*
     * Draw Map
     */
    for (int y = 0; y < game->map.height; y++) {
        for (int x = 0; x < game->map.width; x++) {
            Tile* tile      = &game->map.tiles[y][x];

            Color tileColor = DARKGRAY;

            if (tile->texture_id == TILE_TEXTURE_WALL) {
                tileColor = BLACK;
            } else if (tile->texture_id == TILE_TEXTURE_LEVER_OFF) {
                tileColor = RED;
            } else if (tile->texture_id == TILE_TEXTURE_LEVER_ON) {
                tileColor = GREEN;
            }

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);

            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);
        }
    }

    // Player placeholder
    DrawRectangle(game->player.x, game->player.y, game->player.width, game->player.height, BLUE);

    EndDrawing();
}
