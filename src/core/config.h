/**
 * @file core/config.h
 * @brief Configuration constants.
 */
#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

// Emscripten stuff
#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif

// =============================================================================
// Window and Input Configuration
// =============================================================================

#ifndef DEFAULT_WINDOW_WIDTH
#define DEFAULT_WINDOW_WIDTH 800
#endif

#ifndef DEFAULT_WINDOW_HEIGHT
#define DEFAULT_WINDOW_HEIGHT 600
#endif

#ifndef MENU_DOWN_KEY
#define MENU_DOWN_KEY KEY_DOWN
#endif

#ifndef MENU_UP_KEY
#define MENU_UP_KEY KEY_UP
#endif

#ifndef MENU_SELECT_KEY
#define MENU_SELECT_KEY KEY_ENTER
#endif

// =============================================================================
// Gameplay Configuration
// =============================================================================

#ifndef DEFAULT_LOCALE
#define DEFAULT_LOCALE "en_us.txt"
#endif

#endif
