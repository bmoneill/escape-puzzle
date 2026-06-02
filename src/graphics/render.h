/**
 * @file graphcis/render.h
 * @brief Graphics-library-agnostic rendering API.
 */
#ifndef GRAPHICS_RENDER_H
#define GRAPHICS_RENDER_H

#include "game/game.h"

/**
 * @brief Initialize the renderer
 *
 * This function should be called once at the start of the program to set up any necessary graphics resources.
 */
void render_init(void);

/**
 * @brief Render a single frame of the game
 * @param game The current game state to render
 */
void render_frame(GameState* game);

#endif // GRAPHICS_RENDER_H
