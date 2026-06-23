/**
 * @file puzzles/cipher.c
 * @brief Substitution cipher door puzzle.
 *
 * Each time the player interacts with a cipher door, a fresh random
 * monoalphabetic substitution cipher is generated via Fisher-Yates shuffle.
 * A word is drawn from the pool of riddle answers already in the locale file,
 * encoded with that cipher, then the full 26-letter key table and the encoded
 * word are printed.  The player must reverse the key manually and type the
 * plaintext to open the door.
 */
#include "puzzles/cipher.h"

#include "core/locale.h"
#include "core/log.h"
#include "core/random.h"
#include "core/strings.h"
#include "game/tile.h"
#include "puzzles/puzzle.h"

#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

/**
 * Runs one full cipher puzzle interaction against @p tile.
 * On a correct answer the tile is converted to a floor; the loop otherwise
 * repeats until the player gets it right.
 */
static void do_cipher(Tile* tile) {
    char plaintext[32];
    char locale_key_buf[64];
    char encoded[32];
    char answer_buf[32];

    /* Pick a random riddle answer as the plaintext word. */
    i8 riddle_idx = random_i8_range(1, RIDDLE_COUNT);
    snprintf(locale_key_buf, sizeof(locale_key_buf), "RIDDLE%d_ANSWER", riddle_idx);
    if (!locale_get(locale_key_buf, plaintext, sizeof(plaintext))) {
        log_error_f("Cipher puzzle: failed to load word from locale.\n");
        return;
    }
    s_tolower(plaintext);

    i32 len = (i32) strlen(plaintext);

    /* Build encode_map[0..25]: a random permutation of 'a'..'z'.
     * encode_map[i] is the ciphertext letter for plaintext letter ('a' + i). */
    char encode_map[26];
    for (i32 i = 0; i < 26; i++) {
        encode_map[i] = (char) ('a' + i);
    }
    /* Fisher-Yates shuffle */
    for (i32 i = 25; i > 0; i--) {
        i32  j        = (i32) random_u8_range(0, (u8) i);
        char tmp      = encode_map[i];
        encode_map[i] = encode_map[j];
        encode_map[j] = tmp;
    }

    /* Encode the plaintext; store ciphertext as uppercase letters. */
    for (i32 i = 0; i < len; i++) {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            encoded[i] = (char) (encode_map[plaintext[i] - 'a'] - ('a' - 'A'));
        } else {
            encoded[i] = plaintext[i];
        }
    }
    encoded[len] = '\0';

    /* Display the puzzle.
     *
     * The player reads the encoded word, locates each uppercase letter in the
     * CIPHER row, then reads the corresponding letter from the PLAIN row above
     * it to recover the plaintext.
     *
     * Example (encode_map = QWERTYU...):
     *
     *   Encoded: LQBF
     *
     *   Key (plain -> cipher):
     *   a b c d e f g h i j k l m n o p q r s t u v w x y z
     *   Q W E R T Y U I O P A S D F G H J K L Z X C V B N M
     */
    printf("\n");
    printf("====================================\n");
    printf("CIPHER: Decode the following message.\n");
    printf("\n");
    printf("Encoded: %s\n", encoded);
    printf("\n");
    printf("Key (plain -> cipher):\n");

    for (i32 i = 0; i < 26; i++) {
        printf("%c ", (char) ('a' + i));
    }
    printf("\n");
    for (i32 i = 0; i < 26; i++) {
        printf("%c ", (char) (encode_map[i] - ('a' - 'A')));
    }
    printf("\n\n");

    while (1) {
        printf("Answer: ");
        scanf("%31s", answer_buf);
        s_tolower(answer_buf);

        if (strcmp(answer_buf, plaintext) == 0) {
            printf("Correct! Door opened.\n");
            tile->type       = TILE_FLOOR;
            tile->solid      = 0;
            tile->texture_id = TILE_TEXTURE_FLOOR;
            return;
        }

        printf("Wrong answer. Try again.\n");
    }
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

void cipher_door_spawn(Map* map, i32 x, i32 y) {
    Tile* tile = map_get_tile(map, x, y);
    if (!tile)
        return;

    tile->type       = TILE_CIPHER_DOOR;
    tile->solid      = 1;
    tile->texture_id = TILE_TEXTURE_DOOR;
}

void cipher_door_try(Tile* tile) { do_cipher(tile); }
