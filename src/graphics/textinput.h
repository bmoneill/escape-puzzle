/**
 * @file graphics/textinput.h
 * @brief Graphics-library-agnostic modal text-input interface.
 *
 * Provides a blocking modal overlay used by any puzzle that requires the
 * player to type an answer.  The overlay renders a prompt, an optional body
 * section (e.g., a cipher key table), a text-entry box, and an optional
 * one-line feedback message.  It returns once the player presses Enter.
 *
 * Typical usage (retry loop):
 * @code
 *   const char* feedback = NULL;
 *   while (1) {
 *       textinput_show("RIDDLE\n\nWhat am I?", NULL, feedback, buf, sizeof(buf));
 *       if (correct(buf)) { open_door(); break; }
 *       feedback = "Wrong answer. Try again.";
 *   }
 * @endcode
 */
#ifndef GRAPHICS_TEXTINPUT_H
#define GRAPHICS_TEXTINPUT_H

#include "core/types.h"

/**
 * @brief Displays a modal text-input overlay and blocks until the player presses Enter.
 *
 * @param prompt    Primary text shown at the top of the panel. May contain @c '\\n'
 *                  for explicit line breaks; long lines are word-wrapped automatically.
 *                  Must not be NULL.
 * @param body      Optional supplementary text (e.g., a cipher key table) rendered
 *                  below the prompt in a smaller font. May contain @c '\\n'. Pass
 *                  NULL to omit this section entirely.
 * @param feedback  Optional one-line message drawn below the input box in red (e.g.,
 *                  "Wrong answer. Try again."). Pass NULL on the first call and set
 *                  it after each failed attempt.
 * @param out_buf   Buffer that receives the null-terminated user input on return.
 * @param buf_size  Capacity of @p out_buf in bytes, including the null terminator.
 */
void textinput_show(
    const char* prompt, const char* body, const char* feedback, char* out_buf, i32 buf_size);

/**
 * @brief Briefly displays an "Incorrect!" overlay, then returns.
 *
 * Renders a centered panel with "Incorrect!" in red for a short fixed
 * duration.  The render loop runs normally during this time, so stale
 * key-press state from the previous Enter keypress is cleared before
 * the caller shows the input panel again.
 *
 * Call this after a failed attempt, before re-invoking textinput_show.
 */
void textinput_flash_incorrect(void);

#endif
