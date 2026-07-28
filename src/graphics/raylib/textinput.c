/**
 * @file graphics/raylib/textinput.c
 * @brief Raylib implementation of the modal text-input overlay.
 *
 * Rendering model
 * ---------------
 * When called, this function takes over the render loop entirely (the same
 * pattern used by menu_init).  Each iteration processes input, then draws:
 *
 *   dark background
 *   white panel centered on screen
 *     prompt text   (word-wrapped, FONT_PROMPT)
 *     body text     (word-wrapped, FONT_BODY — optional)
 *     input box     (editable, with blinking cursor)
 *     feedback line (red — optional, shown after a wrong answer)
 *
 * The function returns when the player presses Enter.  If the window is
 * closed, it calls CloseWindow() + exit(), consistent with render_frame().
 *
 * Panel height
 * ------------
 * Computed before the loop from the logical line counts of prompt and body,
 * plus a wrap allowance (+2 logical lines) to accommodate long riddle
 * questions that exceed the panel width.
 */
#include "graphics/textinput.h"

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Layout constants
 * ---------------------------------------------------------------------- */

#define PANEL_W     640 /* fixed panel width in pixels                   */
#define FONT_PROMPT 18 /* font size for the prompt                       */
#define FONT_BODY   14 /* font size for body / supplementary text        */
#define FONT_INPUT  20 /* font size for user-typed text                  */
#define INPUT_H     44 /* height of the text-entry box                   */
#define PAD         24 /* outer and inter-section padding                */
#define LINE_GAP    5 /* extra pixels between successive text lines     */
#define WRAP_FUDGE  2 /* extra logical lines budgeted for word-wrapping */

#define FLASH_PANEL_W  300 /* width of the "Incorrect!" flash panel         */
#define FLASH_PANEL_H  80 /* height of the "Incorrect!" flash panel        */
#define FONT_FLASH     28 /* font size for the flash message               */
#define FLASH_DURATION 1.2f /* seconds the flash panel stays visible        */

/* -------------------------------------------------------------------------
 * Forward declarations for static helpers
 * ---------------------------------------------------------------------- */

static i32 count_lines(const char* text);
static i32
draw_text_multiline(const char* text, i32 x, i32 y, i32 max_w, i32 font_size, Color color);

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

void textinput_flash_incorrect(void) {
    i32   screen_w = GetScreenWidth();
    i32   screen_h = GetScreenHeight();
    i32   panel_x  = (screen_w - FLASH_PANEL_W) / 2;
    i32   panel_y  = (screen_h - FLASH_PANEL_H) / 2;
    float elapsed  = 0.0f;

    while (elapsed < FLASH_DURATION) {
        if (WindowShouldClose()) {
            CloseWindow();
            exit(0);
        }

        elapsed += GetFrameTime();

        BeginDrawing();
        ClearBackground((Color){ 20, 20, 20, 255 });

        DrawRectangle(panel_x, panel_y, FLASH_PANEL_W, FLASH_PANEL_H, WHITE);
        DrawRectangleLines(panel_x, panel_y, FLASH_PANEL_W, FLASH_PANEL_H, DARKGRAY);

        i32 text_w = MeasureText("Incorrect!", FONT_FLASH);
        DrawText("Incorrect!",
                 panel_x + (FLASH_PANEL_W - text_w) / 2,
                 panel_y + (FLASH_PANEL_H - FONT_FLASH) / 2,
                 FONT_FLASH,
                 RED);

        EndDrawing();
    }
}

void textinput_show(
    const char* prompt, const char* body, const char* feedback, char* out_buf, i32 buf_size) {
    i32 len          = 0;
    out_buf[0]       = '\0';
    f32 cursor_timer = 0.0f;

    /* Pre-compute panel height from logical line counts. WRAP_FUDGE gives
     * headroom for long riddle questions that soft-wrap within the panel. */
    i32 prompt_lines   = count_lines(prompt) + WRAP_FUDGE;
    i32 body_lines     = body ? count_lines(body) : 0;
    i32 feedback_lines = feedback ? 1 : 0;

    i32 panel_h   = PAD + prompt_lines * (FONT_PROMPT + LINE_GAP) + (body_lines > 0 ? PAD / 2 : 0)
                    + body_lines * (FONT_BODY + LINE_GAP) + PAD / 2 + INPUT_H
                    + feedback_lines * (FONT_PROMPT + LINE_GAP + 8) + PAD;

    i32 screen_w  = GetScreenWidth();
    i32 screen_h  = GetScreenHeight();
    i32 panel_x   = (screen_w - PANEL_W) / 2;
    i32 panel_y   = (screen_h - panel_h) / 2;
    i32 content_x = panel_x + PAD;
    i32 content_w = PANEL_W - PAD * 2;

    while (1) {
        if (WindowShouldClose()) {
            CloseWindow();
            exit(0);
        }

        /* --- Input -------------------------------------------------------- */

        /* Collect printable characters queued by Raylib this frame. */
        i32 ch = GetCharPressed();
        while (ch > 0) {
            if (ch >= 32 && ch <= 126 && len < buf_size - 1) {
                out_buf[len++] = (char) ch;
                out_buf[len]   = '\0';
            }
            ch = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
            out_buf[--len] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            return;
        }

        /* Blinking cursor: flip every second. */
        cursor_timer += GetFrameTime();
        if (cursor_timer >= 2.0f) {
            cursor_timer -= 2.0f;
        }
        bool show_cursor = cursor_timer < 1.0f;

        /* --- Render ------------------------------------------------------- */

        BeginDrawing();
        ClearBackground((Color){ 20, 20, 20, 255 });

        /* Panel background */
        DrawRectangle(panel_x, panel_y, PANEL_W, panel_h, WHITE);
        DrawRectangleLines(panel_x, panel_y, PANEL_W, panel_h, DARKGRAY);

        i32 cur_y = panel_y + PAD;

        /* Prompt (word-wrapped) */
        cur_y = draw_text_multiline(prompt, content_x, cur_y, content_w, FONT_PROMPT, BLACK);

        /* Body (cipher key table, etc.) */
        if (body) {
            cur_y += PAD / 2;
            cur_y = draw_text_multiline(body, content_x, cur_y, content_w, FONT_BODY, DARKGRAY);
        }

        /* Input box */
        cur_y += PAD / 2;
        DrawRectangle(content_x, cur_y, content_w, INPUT_H, LIGHTGRAY);
        DrawRectangleLines(content_x, cur_y, content_w, INPUT_H, GRAY);

        i32 text_y = cur_y + (INPUT_H - FONT_INPUT) / 2;
        DrawText(out_buf, content_x + 8, text_y, FONT_INPUT, BLACK);

        /* Blinking cursor drawn just after typed text */
        if (show_cursor) {
            i32 text_w = MeasureText(out_buf, FONT_INPUT);
            DrawText("|", content_x + 8 + text_w, text_y, FONT_INPUT, DARKGRAY);
        }

        /* Feedback (wrong-answer message) */
        if (feedback) {
            cur_y += INPUT_H + 8;
            DrawText(feedback, content_x, cur_y, FONT_PROMPT, RED);
        }

        EndDrawing();
    }
}

/* -------------------------------------------------------------------------
 * Static helpers
 * ---------------------------------------------------------------------- */

/** Returns the number of newline-delimited lines in @p text (0 if empty). */
static i32 count_lines(const char* text) {
    if (!text || *text == '\0') {
        return 0;
    }
    i32 count = 1;
    for (const char* p = text; *p; p++) {
        if (*p == '\n') {
            count++;
        }
    }
    return count;
}

/**
 * Draws @p text with word-wrapping at @p max_w pixels.  Explicit @c '\\n'
 * characters force a line break.  Returns the y coordinate directly below
 * the last drawn line.
 */
static i32
draw_text_multiline(const char* text, i32 x, i32 y, i32 max_w, i32 font_size, Color color) {
    char line[256];
    i32  line_len = 0;
    i32  cur_y    = y;

    line[0]       = '\0';

    const char* p = text;
    while (*p) {
        /* Explicit newline: flush current line and move down. */
        if (*p == '\n') {
            line[line_len] = '\0';
            DrawText(line, x, cur_y, font_size, color);
            cur_y += font_size + LINE_GAP;
            line[0]  = '\0';
            line_len = 0;
            p++;
            continue;
        }

        /* Find end of next word (up to next space or newline). */
        const char* word_end = p;
        while (*word_end && *word_end != ' ' && *word_end != '\n') {
            word_end++;
        }
        i32 word_len = (i32) (word_end - p);

        /* Build the candidate string: existing line + space + new word. */
        char cand[256];
        if (line_len > 0) {
            snprintf(cand, sizeof(cand), "%s %.*s", line, word_len, p);
        } else {
            snprintf(cand, sizeof(cand), "%.*s", word_len, p);
        }

        if (MeasureText(cand, font_size) > max_w && line_len > 0) {
            /* Word doesn't fit: flush current line, start a new one. */
            DrawText(line, x, cur_y, font_size, color);
            cur_y += font_size + LINE_GAP;
            snprintf(line, sizeof(line), "%.*s", word_len, p);
            line_len = (i32) strlen(line);
        } else {
            /* Word fits: append candidate to line. */
            snprintf(line, sizeof(line), "%s", cand);
            line_len = (i32) strlen(line);
        }

        p = word_end;
        if (*p == ' ') {
            p++; /* skip the trailing space between words */
        }
    }

    /* Flush the final line. */
    if (line_len > 0) {
        DrawText(line, x, cur_y, font_size, color);
        cur_y += font_size + LINE_GAP;
    }

    return cur_y;
}
