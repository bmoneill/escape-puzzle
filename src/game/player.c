/**
 * @file game/player.c
 * @brief Player implementation.
 */
#include "player.h"

#include "graphics/input.h"

#include <stdlib.h>

void player_init(Player* player) {
    player->x          = 64;
    player->y          = 64;

    player->move_speed = DEFAULT_PLAYER_SPEED;

    player->width      = 32;
    player->height     = 32;
}

void player_update(Player* player, Map* map, i16 keys_pressed) {
    if (keys_pressed & GAME_KEY_EXIT) {
        exit(0);
    }

    player_move(player, map, keys_pressed);
}

void player_move(Player* player, Map* map, i16 keys) {
    f64  dx = 0;
    f64  dy = 0;
    f64  nextX, nextY;
    i32  leftTileX, rightTileX, topTileY, bottomTileY;
    bool canMoveX = true, canMoveY = true;

    if (keys & GAME_KEY_UP) {
        dy -= player->move_speed;
    }

    if (keys & GAME_KEY_DOWN) {
        dy += player->move_speed;
    }

    if (keys & GAME_KEY_LEFT) {
        dx -= player->move_speed;
    }

    if (keys & GAME_KEY_RIGHT) {
        dx += player->move_speed;
    }

    if (dx || dy) {
        nextX       = (player->x + dx) / TILE_SIZE;
        nextY       = (player->y + dy) / TILE_SIZE;
        leftTileX   = (i32) nextX;
        rightTileX  = (i32) (nextX + 1);
        topTileY    = (i32) nextY;
        bottomTileY = (i32) (nextY + 1);
    }

    if (dx) {
        i32 checkX = (dx > 0) ? rightTileX : leftTileX;
        for (i32 tileY = topTileY; tileY <= bottomTileY; tileY++) {
            Tile* tile = map_get_tile(map, checkX, tileY);
            if (tile && tile->solid) {
                canMoveX = false;
                break;
            }
        }

        if (canMoveX) {
            player->x += dx;
        }
    }

    if (dy) {
        i32 checkY = (dy > 0) ? bottomTileY : topTileY;
        for (i32 tileX = leftTileX; tileX <= rightTileX; tileX++) {
            Tile* tile = map_get_tile(map, tileX, checkY);
            if (tile && tile->solid) {
                canMoveY = false;
                break;
            }
        }

        if (canMoveY) {
            player->y += dy;
        }
    }
}
