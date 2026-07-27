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

// Yield to the browser event loop once per frame when running as WASM.
// Requires -sASYNCIFY=1 at link time.  Compiles to a no-op on all other
// platforms so callers do not need any conditional compilation.
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define WASM_YIELD() emscripten_sleep(0)
#else
#define WASM_YIELD() ((void) 0)
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
