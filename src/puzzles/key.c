#include "key.h"
#include "game/tile.h"

void key_spawn(Map* map, i32 x, i32 y) {
    Tile* tile = map_get_tile(map, x, y);
    if (!tile)
        return;

    tile->type       = TILE_KEY;
    tile->solid      = 0;
    tile->texture_id = TILE_TEXTURE_KEY;
}

void key_try_pickup(Map* map, Player* player) {
    i32 px = (i32) (player->x / TILE_SIZE);
    i32 py = (i32) (player->y / TILE_SIZE);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            Tile* tile = map_get_tile(map, px + dx, py + dy);
            if (!tile)
                continue;

            if (tile->type == TILE_KEY) {
                player->keys += 1;

                tile->type       = TILE_FLOOR;
                tile->solid      = 0;
                tile->texture_id = TILE_TEXTURE_FLOOR;
                return;
            }
        }
    }
}
