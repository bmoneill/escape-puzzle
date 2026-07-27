/**
 * @file game/player.c
 * @brief Player implementation.
 */
#include "player.h"
#include "core/config.h"
#include "puzzles/door.h"
#include "puzzles/key.h"

#include "graphics/input.h"

#include <stdlib.h>

EMSCRIPTEN_KEEPALIVE void player_init(Player* player) {
    player->x          = 64;
    player->y          = 64;

    player->move_speed = DEFAULT_PLAYER_SPEED;

    player->width      = 20;
    player->height     = 20;
    player->keys       = 0;

    player->facing     = PLAYER_DIR_DOWN;
    player->is_moving  = false;
    player->anim_tick  = 0;
}

EMSCRIPTEN_KEEPALIVE void player_update(Player* player, Map* map, i16 keys_pressed) {
    if (keys_pressed & GAME_KEY_EXIT) {
        exit(0);
    }
    if (keys_pressed & GAME_KEY_INTERACT) {
        key_try_pickup(map, player);
        door_try_open(map, player);
    }

    player_move(player, map, keys_pressed);
}

EMSCRIPTEN_KEEPALIVE void player_move(Player* player, Map* map, i16 keys) {
    f64 dx = 0;
    f64 dy = 0;

    if (keys & GAME_KEY_UP)
        dy -= player->move_speed;
    if (keys & GAME_KEY_DOWN)
        dy += player->move_speed;
    if (keys & GAME_KEY_LEFT)
        dx -= player->move_speed;
    if (keys & GAME_KEY_RIGHT)
        dx += player->move_speed;

    // Update facing direction (vertical takes priority over horizontal)
    if (dy < 0)
        player->facing = PLAYER_DIR_UP;
    else if (dy > 0)
        player->facing = PLAYER_DIR_DOWN;
    else if (dx < 0)
        player->facing = PLAYER_DIR_LEFT;
    else if (dx > 0)
        player->facing = PLAYER_DIR_RIGHT;

    // Advance animation tick while moving; reset to 0 when idle
    if (dx != 0 || dy != 0) {
        player->is_moving = true;
        player->anim_tick++;
    } else {
        player->is_moving = false;
        player->anim_tick = 0;
    }

    // Horizontal movement ---------------------------------------------------
    // Check the leading X edge of the moved bounding box against every tile
    // row the player currently occupies.  Using the *current* Y (not moved Y)
    // keeps the two axes independent and prevents corner-clipping artifacts.
    if (dx) {
        f64  nextLeft  = player->x + dx;
        f64  nextRight = player->x + dx + player->width - 1;
        i32  checkX    = (dx > 0) ? (i32) (nextRight / TILE_SIZE) : (i32) (nextLeft / TILE_SIZE);
        i32  topTile   = (i32) (player->y / TILE_SIZE);
        i32  botTile   = (i32) ((player->y + player->height - 1) / TILE_SIZE);

        bool blocked   = false;
        for (i32 ty = topTile; ty <= botTile; ty++) {
            Tile* tile = map_get_tile(map, checkX, ty);
            if (tile && tile->solid) {
                blocked = true;
                break;
            }
        }
        if (!blocked)
            player->x += dx;
    }

    // Vertical movement -----------------------------------------------------
    // Check the leading Y edge against every tile column the player currently
    // occupies (after the X move has already been applied above).
    if (dy) {
        f64  nextTop   = player->y + dy;
        f64  nextBot   = player->y + dy + player->height - 1;
        i32  checkY    = (dy > 0) ? (i32) (nextBot / TILE_SIZE) : (i32) (nextTop / TILE_SIZE);
        i32  leftTile  = (i32) (player->x / TILE_SIZE);
        i32  rightTile = (i32) ((player->x + player->width - 1) / TILE_SIZE);

        bool blocked   = false;
        for (i32 tx = leftTile; tx <= rightTile; tx++) {
            Tile* tile = map_get_tile(map, tx, checkY);
            if (tile && tile->solid) {
                blocked = true;
                break;
            }
        }
        if (!blocked)
            player->y += dy;
    }
}
