/**
 * @file graphics/raylib/render.c
 * @brief Raylib implementation of the rendering system.
 */
#include "graphics/render.h"

#include "game/game.h"
#include "game/map.h"
#include "game/player.h"
#include "game/tile.h"
#include "puzzles/puzzle.h"
#include "puzzles/lever.h"
#include "raylib.h"

#define TILE_SIZE            32
#define SPRITE_SIZE          48
#define ANIM_FRAMES_PER_STEP 10

static Texture2D player_spritesheet;

void render_init(void) {
    InitWindow(800, 600, "Escape Puzzle");
    SetTargetFPS(60);
    player_spritesheet = LoadTexture("assets/spritesheet.png");
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

/**
 * @brief Draws the player sprite from the spritesheet with walking animation.
 *
 * Spritesheet layout (each cell is SPRITE_SIZE x SPRITE_SIZE):
 *   col 0, row 0 = standing still, facing down
 *   col 1, row 0 = walking, facing down
 *   col 0, row 1 = standing still, facing up
 *   col 1, row 1 = walking, facing up
 *   col 0, row 2 = standing still, facing left  (right is the same, flipped)
 *   col 1, row 2 = walking, facing left          (right is the same, flipped)
 *
 * Animation sequences:
 *   Up / Down  : still -> walk -> still -> walk (h-flipped) -> repeat
 *   Left / Right: still -> walk -> repeat
 */
static void draw_player(const Player* player) {
    int  col    = 0;
    int  row    = 0;
    bool flip_x = false;

    switch (player->facing) {
        case PLAYER_DIR_DOWN:  row = 0;                  break;
        case PLAYER_DIR_UP:    row = 1;                  break;
        case PLAYER_DIR_LEFT:  row = 2;                  break;
        case PLAYER_DIR_RIGHT: row = 2; flip_x = true;  break;
    }

    if (player->is_moving) {
        if (player->facing == PLAYER_DIR_UP || player->facing == PLAYER_DIR_DOWN) {
            // 4-phase cycle: still(0) -> walk(1) -> still(2) -> walk-flipped(3)
            u32 phase = (player->anim_tick / ANIM_FRAMES_PER_STEP) % 4;
            col = (phase == 1 || phase == 3) ? 1 : 0;
            if (phase == 3)
                flip_x = !flip_x;
        } else {
            // 2-phase cycle: still(0) -> walk(1)
            u32 phase = (player->anim_tick / ANIM_FRAMES_PER_STEP) % 2;
            col       = (int) phase;
        }
    }

    // For a horizontal flip, raylib uses a negative source width with x at the
    // right edge of the cell.
    Rectangle src = {
        .x      = (float)(col * SPRITE_SIZE),
        .y      = (float)(row * SPRITE_SIZE),
        .width  = flip_x ? -(float)SPRITE_SIZE : (float)SPRITE_SIZE,
        .height = (float)SPRITE_SIZE,
    };

    // Centre the sprite over the player's collision box
    float draw_x = (float) player->x - (SPRITE_SIZE - player->width) / 2.0f;
    float draw_y = (float) player->y - (SPRITE_SIZE - player->height) / 2.0f;

    DrawTextureRec(player_spritesheet, src, (Vector2){ draw_x, draw_y }, WHITE);
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
            } else if (tile->texture_id == TILE_TEXTURE_EXIT) {
                tileColor = ORANGE;
            } else if (tile->texture_id == TILE_TEXTURE_HIDDEN_LEVER) {
                tileColor = WHITE; // temporary
            }

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);

            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);

             draw_lever_number(game, x, y);

            DrawText(TextFormat ("Keys: %d", game->player.keys), 10, 10, 20, WHITE);
        }
    }

    draw_player(&game->player);

    EndDrawing();
}
