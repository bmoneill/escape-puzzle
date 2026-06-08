#include "door.h"
#include "game/tile.h"

void door_spawn(Map* map, i32 x, i32 y) {
    Tile* tile = map_get_tile(map, x, y);
    if (!tile)
        return;

    tile->type       = TILE_DOOR;
    tile->solid      = 1;
    tile->texture_id = TILE_TEXTURE_DOOR;
}

void door_try_open(Map* map, Player* player) {
    i32 px = (i32) (player->x / TILE_SIZE);
    i32 py = (i32) (player->y / TILE_SIZE);

    // check adjacent tiles
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            Tile* tile = map_get_tile(map, px + dx, py + dy);
            if (!tile)
                continue;

            if (tile->type == TILE_DOOR && player->keys > 0) {
                player->keys--;

                tile->type       = TILE_FLOOR;
                tile->solid      = 0;
                tile->texture_id = 0;
                return;
            }
        }
    }
}
