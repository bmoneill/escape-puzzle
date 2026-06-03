#ifndef GAME_PUZZLE_H
#define GAME_PUZZLE_H

#include "core/types.h"

#ifndef MAX_PUZZLES
#define MAX_PUZZLES 10
#endif

typedef enum { PUZZLE_LEVER_TOGGLE } PuzzleType;

typedef enum {
    PUZZLE_EVENT_PLAYER_INTERACT,
} PuzzleEventType;

typedef struct {
    vec2u      position;
    bool       completed;
    PuzzleType type;

    void (*update)(void* self, void* state);
} Puzzle;

typedef struct {
    i32             puzzleIdx;
    PuzzleEventType type;
} PuzzleEvent;

typedef struct {
    bool isOn;
} LeverTogglePuzzleState;

/**
 * @brief Generates puzzle events based on the player's position and interactions.
 * @param playerX player X coordinate in tile units.
 * @param playerY player Y coordinate in tile units.
 * @param keys_pressed The current key inputs from the player.
 * @param puzzles The array of puzzles in the game.
 * @param num_puzzles The number of puzzles in the array.
 * @param out_num_events Output parameter to store the number of generated events.
 * @return An array of PuzzleEvent structures representing the events that occurred.
 */
PuzzleEvent* puzzle_generate_events(i32     playerX,
                                    i32     playerY,
                                    i16     keys_pressed,
                                    Puzzle* puzzles,
                                    i32     num_puzzles,
                                    i32*    out_num_events);

/**
 * @brief Updates the state of all puzzles based on the generated events.
 * @param puzzles The array of puzzles to update.
 * @param num_puzzles The number of puzzles in the array.
 * @param events The array of PuzzleEvent structures representing the events that occurred.
 * @param num_events The number of events in the array.
 * @return true if all puzzles are completed, false otherwise.
 */
bool puzzle_update(Puzzle* puzzles, i32 num_puzzles, PuzzleEvent* events, i32 num_events);

/**
 * @brief Update function for a lever toggle puzzle, which toggles the puzzle's completion state based on the provided state.
 * @param self Pointer to the Puzzle instance being updated.
 * @param state Pointer to the LeverTogglePuzzleState.
 */
void lever_toggle_puzzle_update(void* self, void* state);
#endif
