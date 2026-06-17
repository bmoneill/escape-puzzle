#include "door.h"
#include "game/tile.h"
#include <stdio.h>
#include <string.h>

void door_spawn(Map* map, i32 x, i32 y) {
    Tile* tile = map_get_tile(map, x, y);
    if (!tile)
        return;

    tile->type       = TILE_DOOR;
    tile->solid      = 1;
    tile->texture_id = TILE_TEXTURE_DOOR;
}

void door_try_open(Map* map, Player* player) {
    i32 px = (i32)(player->x / TILE_SIZE);
    i32 py = (i32)(player->y / TILE_SIZE);

    // check adjacent tiles
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            Tile* tile = map_get_tile(map, px + dx, py + dy);

            if (!tile)
                continue;

            /* Normal door -> requires key */
            if (tile->type == TILE_DOOR) {

                if (player->keys <= 0)
                    return;

                player->keys--;

                tile->type       = TILE_FLOOR;
                tile->solid      = 0;
                tile->texture_id = TILE_TEXTURE_FLOOR;

                return;
            }

            /* Riddle door */
            if (tile->type == TILE_RIDDLE_DOOR) {

                char answer[64];

                printf("\n");
                printf("====================================\n");
                printf("RIDDLE:\n");
                printf("What has keys but can't open locks?\n");
                printf("Answer: ");

                scanf("%63s", answer);

                if (strcmp(answer, "keyboard") == 0 ||
                    strcmp(answer, "Keyboard") == 0) {

                    printf("Correct! Door opened.\n");

                    tile->type       = TILE_FLOOR;
                    tile->solid      = 0;
                    tile->texture_id = TILE_TEXTURE_FLOOR;
                } else {
                    printf("Wrong answer.\n");
                }

                return;
            }
        }
    }
}

void riddle_door_spawn(Map* map, i32 x, i32 y) {
    Tile* tile = map_get_tile(map, x, y);

    if (!tile)
        return;

    tile->type       = TILE_RIDDLE_DOOR;
    tile->solid      = 1;
    tile->texture_id = TILE_TEXTURE_DOOR;
}

void door_try_riddle(Map* map, Player* player) {
    i32 px = (i32)(player->x / TILE_SIZE);
    i32 py = (i32)(player->y / TILE_SIZE);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            Tile* tile = map_get_tile(map, px + dx, py + dy);

            if (!tile)
                continue;

            if (tile->type != TILE_DOOR)
                continue;

            char answer[64];

            printf("\n");
            printf("====================================\n");
            printf("RIDDLE:\n");
            printf("What has keys but can't open locks?\n");
            printf("Answer: ");

            scanf("%63s", answer);

            if (strcmp(answer, "keyboard") == 0 ||
                strcmp(answer, "Keyboard") == 0) {

                printf("Correct! Door opened.\n");

                tile->type       = TILE_FLOOR;
                tile->solid      = 0;
                tile->texture_id = TILE_TEXTURE_FLOOR;
            } else {
                printf("Wrong answer.\n");
            }

            return;
        }
    }
}
