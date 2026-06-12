/**
 * @file graphics/raylib/render.c
 * @brief Raylib implementation of the rendering system.
 */
#include "graphics/render.h"

#include "game/game.h"
#include "game/map.h"
#include "game/tile.h"
#include "puzzles/puzzle.h"
#include "puzzles/lever.h"
#include "raylib.h"

#define TILE_SIZE 32

void render_init(void) {
    InitWindow(800, 600, "Escape Puzzle");
    SetTargetFPS(60);
}

static void draw_lever_number(GameState* game, int x, int y) {
    Tile* tile = &game->map.tiles[y][x];

    if (tile->texture_id != TILE_TEXTURE_LEVER_OFF &&
        tile->texture_id != TILE_TEXTURE_LEVER_ON) {
        return;
    }

    // find matching puzzle
    for (int i = 0; i < game->map.num_puzzles; i++) {

        Puzzle* p = &game->map.puzzles[i];

        if (p->position.x == (unsigned int)x && p->position.y == (unsigned int)y) {

            LeverState* state = (LeverState*)p->state;

            if (state) {
                DrawText(
                    TextFormat("%d", state->order),
                    x * TILE_SIZE + 10,
                    y * TILE_SIZE + 8,
                    20,
                    WHITE
                );
            }
        }
    }
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
            } else if  (tile->texture_id == TILE_TEXTURE_DOOR) {
                tileColor = PURPLE;
            } else if  (tile->texture_id == TILE_TEXTURE_KEY) {
                tileColor = YELLOW;
            }

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);

            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);

             draw_lever_number(game, x, y);

            DrawText(TextFormat ("Keys: %d", game->player.keys), 10, 10, 20, WHITE);
        }
    }

    // Player placeholder
    DrawRectangle(game->player.x, game->player.y, game->player.width, game->player.height, BLUE);

    EndDrawing();
}
