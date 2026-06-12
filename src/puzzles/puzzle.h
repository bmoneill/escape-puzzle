#ifndef GAME_PUZZLE_H
#define GAME_PUZZLE_H

#include "core/types.h"

#ifndef MAX_PUZZLES
#define MAX_PUZZLES 10
#endif

/**
 * @brief Defines the different types of puzzles in the game.
 */
typedef enum { PUZZLE_LEVER_TOGGLE } PuzzleType;

/**
 * @brief Defines the different types of events that can occur related to puzzles.
 */
typedef enum {
    PUZZLE_EVENT_PLAYER_INTERACT,
} PuzzleEventType;

/**
 * @brief Represents an event related to a puzzle, such as the player interacting with it.
 */
typedef struct {
    i32             puzzleIdx; //!< The index of the puzzle this event is related to
    PuzzleEventType type; //!< The type of event that occurred
    void* data; //!< Optional data for the event, can be used for more complex interactions
    bool  handled;
} PuzzleEvent;

/**
 * @brief Lever specific state
 */

typedef struct {
    i32 order;
    bool activated;
} LeverState;

/**
 * @brief Represents a puzzle in the game, including its position, completion state, type, and update function.
 */
typedef struct {
    vec2u      position; //!< The position of the puzzle in tile coordinates
    bool       completed; //!< Whether the puzzle has been completed or not
    PuzzleType type; //!< The type of the puzzle, which determines its behavior and update logic
    void*      state; //!< Optional state data for the puzzle, can be used for more complex puzzles
    void (*update)(void* self, PuzzleEvent* event); //!< Update function
} Puzzle;

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

#endif
