#include "game/puzzle.h"

#include "core/log.h"
#include "core/memory.h"
#include "game/map.h"
#include "graphics/input.h"

void lever_toggle_puzzle_update(void* self, void* state) {
    Puzzle*                 puzzle      = (Puzzle*) self;
    LeverTogglePuzzleState* puzzleState = (LeverTogglePuzzleState*) state;

    if (puzzleState->isOn) {
        puzzle->completed                                              = true;
        gmap->tiles[puzzle->position.y][puzzle->position.x].texture_id = TILE_TEXTURE_LEVER_ON;
        log_info_f("Puzzle at (%d, %d) is complete", puzzle->position.x, puzzle->position.y);
    }
}

PuzzleEvent* puzzle_generate_events(i32     playerX,
                                    i32     playerY,
                                    i16     keys_pressed,
                                    Puzzle* puzzles,
                                    i32     num_puzzles,
                                    i32*    out_num_events) {
    PuzzleEvent* events = MEM_FRAME(sizeof(PuzzleEvent) * num_puzzles);
    i32          count  = 0;

    if (keys_pressed & GAME_KEY_INTERACT) {
        for (i32 i = 0; i < num_puzzles; i++) {
            Puzzle* puzzle = &puzzles[i];
            i32     dx     = playerX - puzzle->position.x;
            i32     dy     = playerY - puzzle->position.y;

            if (dx < 0) {
                dx = -dx;
            }
            if (dy < 0) {
                dy = -dy;
            }

            if (dx < 3 && dy < 3) {
                // TODO consider that count might exceed num_puzzles
                events[count++]
                    = (PuzzleEvent) { .puzzleIdx = i, .type = PUZZLE_EVENT_PLAYER_INTERACT };
            }
        }
    }

    *out_num_events = count;
    return events;
}

bool puzzle_update(Puzzle* puzzles, i32 num_puzzles, PuzzleEvent* events, i32 num_events) {
    i32 num_completed = 0;
    for (i32 i = 0; i < num_puzzles; i++) {
        Puzzle* puzzle = &puzzles[i];

        if (puzzle->completed) {
            num_completed++;
            continue;
        }

        LeverTogglePuzzleState state = { .isOn = false };

        for (i32 j = 0; j < num_events; j++) {
            PuzzleEvent* event = &events[j];

            if (event->type == PUZZLE_EVENT_PLAYER_INTERACT && event->puzzleIdx == i) {
                state.isOn = true;
            }
        }

        puzzle->update(puzzle, &state);
    }

    if (num_completed == num_puzzles) {
        return true;
    }

    return false;
}
