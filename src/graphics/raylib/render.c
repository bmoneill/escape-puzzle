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
#include "raylib.h"

#define TILE_SIZE            32
#define SPRITE_SIZE          48
#define ANIM_FRAMES_PER_STEP 10

/*
 * Static-world sprite locations within the spritesheet (col, row of
 * SPRITE_SIZE x SPRITE_SIZE cells). See draw_player()'s comment for the
 * player animation cells.
 *
 *   col 2, row 0 = wall stone tile
 *   col 4, row 0 = floor wood tile
 *   col 3, row 0 = key (drawn on top of the floor texture, has transparency)
 *   col 2, row 1 = lever, activated,   sitting on a floor (wood backdrop baked in)
 *   col 2, row 2 = lever, deactivated, sitting on a floor (wood backdrop baked in)
 *   col 3, row 1 = lever, activated,   sitting on a wall  (stone backdrop baked in)
 *   col 3, row 2 = lever, deactivated, sitting on a wall  (stone backdrop baked in)
 */
#define SPRITE_COL_WALL            2
#define SPRITE_ROW_WALL            0
#define SPRITE_COL_FLOOR           4
#define SPRITE_ROW_FLOOR           0
#define SPRITE_COL_KEY             3
#define SPRITE_ROW_KEY             0
#define SPRITE_COL_LEVER_FLOOR     2
#define SPRITE_ROW_LEVER_FLOOR_ON  1
#define SPRITE_ROW_LEVER_FLOOR_OFF 2
#define SPRITE_COL_LEVER_WALL      3
#define SPRITE_ROW_LEVER_WALL_ON   1
#define SPRITE_ROW_LEVER_WALL_OFF  2

static Texture2D spritesheet;

void             render_init(void) {
    InitWindow(800, 600, "Puzzle Realm");
    SetTargetFPS(60);
    spritesheet = LoadTexture("assets/spritesheet.png");
}

/**
 * @brief Draws a single SPRITE_SIZE x SPRITE_SIZE cell of the spritesheet,
 * scaled to fill a TILE_SIZE x TILE_SIZE destination square on-screen.
 */
static void draw_sprite_cell(int col, int row, int x, int y) {
    Rectangle src
        = { (float) (col * SPRITE_SIZE), (float) (row * SPRITE_SIZE), SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dst = { (float) (x * TILE_SIZE), (float) (y * TILE_SIZE), TILE_SIZE, TILE_SIZE };
    DrawTexturePro(spritesheet, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
}

/**
 * @brief Finds the puzzle registered at the given tile coordinates, if any.
 */
static Puzzle* find_puzzle_at(GameState* game, int x, int y) {
    for (int i = 0; i < game->map.num_puzzles; i++) {
        Puzzle* p = &game->map.puzzles[i];
        if (p->position.x == (unsigned int) x && p->position.y == (unsigned int) y) {
            return p;
        }
    }
    return NULL;
}

static void draw_lever_number(GameState* game, int x, int y) {
    Tile* tile = &game->map.tiles[y][x];

    if (tile->texture_id != TILE_TEXTURE_LEVER_OFF && tile->texture_id != TILE_TEXTURE_LEVER_ON) {
        return;
    }

    Puzzle* p = find_puzzle_at(game, x, y);

    if (p) {
        LeverState* state = (LeverState*) p->state;

        if (state) {
            DrawText(TextFormat("%d", state->order),
                     x * TILE_SIZE + 10,
                     y * TILE_SIZE + 8,
                     20,
                     WHITE);
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
    case PLAYER_DIR_DOWN:
        row = 0;
        break;
    case PLAYER_DIR_UP:
        row = 1;
        break;
    case PLAYER_DIR_LEFT:
        row = 2;
        break;
    case PLAYER_DIR_RIGHT:
        row    = 2;
        flip_x = true;
        break;
    }

    if (player->is_moving) {
        if (player->facing == PLAYER_DIR_UP || player->facing == PLAYER_DIR_DOWN) {
            // 4-phase cycle: still(0) -> walk(1) -> still(2) -> walk-flipped(3)
            u32 phase = (player->anim_tick / ANIM_FRAMES_PER_STEP) % 4;
            col       = (phase == 1 || phase == 3) ? 1 : 0;
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
        .x      = (float) (col * SPRITE_SIZE),
        .y      = (float) (row * SPRITE_SIZE),
        .width  = flip_x ? -(float) SPRITE_SIZE : (float) SPRITE_SIZE,
        .height = (float) SPRITE_SIZE,
    };

    // Centre the sprite over the player's collision box
    float draw_x = (float) player->x - (SPRITE_SIZE - player->width) / 2.0f;
    float draw_y = (float) player->y - (SPRITE_SIZE - player->height) / 2.0f;

    DrawTextureRec(spritesheet, src, (Vector2){ draw_x, draw_y }, WHITE);
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
            Tile* tile = &game->map.tiles[y][x];

            switch (tile->texture_id) {
            case TILE_TEXTURE_FLOOR:
                draw_sprite_cell(SPRITE_COL_FLOOR, SPRITE_ROW_FLOOR, x, y);
                break;

            case TILE_TEXTURE_WALL:
                draw_sprite_cell(SPRITE_COL_WALL, SPRITE_ROW_WALL, x, y);
                break;

            case TILE_TEXTURE_KEY:
                // The key sprite has transparent parts, so draw the floor
                // underneath it first.
                draw_sprite_cell(SPRITE_COL_FLOOR, SPRITE_ROW_FLOOR, x, y);
                draw_sprite_cell(SPRITE_COL_KEY, SPRITE_ROW_KEY, x, y);
                break;

            case TILE_TEXTURE_LEVER_ON:
                // Regular (non-hidden) levers sit on a floor tile, and their
                // sprite already includes the wood floor backdrop.
                draw_sprite_cell(SPRITE_COL_LEVER_FLOOR, SPRITE_ROW_LEVER_FLOOR_ON, x, y);
                break;

            case TILE_TEXTURE_LEVER_OFF:
                draw_sprite_cell(SPRITE_COL_LEVER_FLOOR, SPRITE_ROW_LEVER_FLOOR_OFF, x, y);
                break;

            case TILE_TEXTURE_HIDDEN_LEVER: {
                // Hidden levers are embedded in a wall tile, and their sprite
                // already includes the stone wall backdrop, so it can be
                // drawn directly. Pick on/off based on the puzzle's state.
                bool    activated = false;
                Puzzle* p         = find_puzzle_at(game, x, y);

                if (p) {
                    LeverState* state = (LeverState*) p->state;
                    if (state)
                        activated = state->activated;
                }

                int row = activated ? SPRITE_ROW_LEVER_WALL_ON : SPRITE_ROW_LEVER_WALL_OFF;
                draw_sprite_cell(SPRITE_COL_LEVER_WALL, row, x, y);
                break;
            }

            case TILE_TEXTURE_DOOR:
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PURPLE);
                break;

            case TILE_TEXTURE_EXIT:
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, ORANGE);
                break;

            default:
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
                break;
            }

            draw_lever_number(game, x, y);

            DrawText(TextFormat("Keys: %d", game->player.keys), 10, 10, 20, WHITE);
        }
    }

    draw_player(&game->player);

    EndDrawing();
}
