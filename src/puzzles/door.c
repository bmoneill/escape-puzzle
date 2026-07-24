#include "door.h"
#include "core/locale.h"
#include "core/log.h"
#include "core/random.h"
#include "core/strings.h"
#include "game/tile.h"
#include "graphics/textinput.h"
#include "puzzles/cipher.h"
#include <stdio.h>
#include <string.h>

static void do_riddle(Tile*);

void        door_spawn(Map* map, i32 x, i32 y) {
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
                do_riddle(tile);
            }

            /* Cipher door */
            if (tile->type == TILE_CIPHER_DOOR) {
                cipher_door_try(tile);
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

static void do_riddle(Tile* tile) {
    char question[128];
    char answer_buf[64];
    char correct_answer[64];
    char locale_key_buf[64];
    i8   riddleIdx = random_i8_range(0, RIDDLE_COUNT);

    snprintf(locale_key_buf, 64, "RIDDLE%d_QUESTION", riddleIdx);
    if (!locale_get(locale_key_buf, question, 128)) {
        log_error_f("Failed to load riddle.\n");
        return;
    }

    snprintf(locale_key_buf, 64, "RIDDLE%d_ANSWER", riddleIdx);

    if (!locale_get(locale_key_buf, correct_answer, 64)) {
        log_error_f("Failed to load riddle answer.\n");
        return;
    }

    s_tolower(correct_answer);

    char prompt[192];
    snprintf(prompt, sizeof(prompt), "RIDDLE\n\n%s", question);

    while (1) {
        textinput_show(prompt, NULL, NULL, answer_buf, sizeof(answer_buf));
        s_tolower(answer_buf);

        if (strcmp(answer_buf, correct_answer) == 0) {
            tile->type       = TILE_FLOOR;
            tile->solid      = 0;
            tile->texture_id = TILE_TEXTURE_FLOOR;
            return;
        }

        textinput_flash_incorrect();
    }
}
