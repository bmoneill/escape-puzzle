/**
 * @file game/player.c
 * @brief Player implementation.
 */
#include "player.h"

#include "graphics/input.h"

#include <stdlib.h>

#define TILE_SIZE 32

void player_init(Player* player)
{
    player->x = 64;
    player->y = 64;

    player->move_speed = DEFAULT_PLAYER_SPEED;

    player->width = 32;
    player->height = 32;
}

void player_update(Player* player, Map* map)
{
    i16 keys_pressed = get_keys_pressed();

    if (keys_pressed & GAME_KEY_EXIT)
    {
        exit(0);
    }

    player_move(player, map, keys_pressed);
}

void player_move(Player* player, Map* map, i16 keys)
{
    float dx = 0;
    float dy = 0;

    if (keys & GAME_KEY_UP)
    {
        dy -= player->move_speed;
    }

    if (keys & GAME_KEY_DOWN)
    {
        dy += player->move_speed;
    }

    if (keys & GAME_KEY_LEFT)
    {
        dx -= player->move_speed;
    }

    if (keys & GAME_KEY_RIGHT)
    {
        dx += player->move_speed;
    }

    /*
     * X movement
     */
    float nextX = player->x + dx;

    int tileX = (int)(nextX + player->width / 2) / TILE_SIZE;
    int tileY = (int)(player->y + player->height / 2) / TILE_SIZE;

    Tile* tile = map_get_tile(map, tileX, tileY);

    if (tile && !tile->solid)
    {
        player->x = nextX;
    }

    /*
     * Y movement
     */
    float nextY = player->y + dy;

    tileX = (int)(player->x + player->width / 2) / TILE_SIZE;
    tileY = (int)(nextY + player->height / 2) / TILE_SIZE;

    tile = map_get_tile(map, tileX, tileY);

    if (tile && !tile->solid)
    {
        player->y = nextY;
    }
}