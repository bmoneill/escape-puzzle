#ifndef GRAPHICS_MENU_H
#define GRAPHICS_MENU_H

#include "core/types.h"
#include "game/game.h"

/**
 * @brief Represents a single menu item, including its display name and the action to perform when selected.
 */
typedef struct {
    const char* name; //!< The display text of the menu item.
    void (*action)(
        GameState*
            game); //!< A function pointer to the action to execute when this menu item is selected.
} MenuItem;

/**
 * @brief Represents a menu.
 */
typedef struct {
    MenuItem* items; //!< An array of menu items to display in the menu.
    i32       num_items; //!< The number of items in the menu.
    i32       selected_menu_item; //!< The index of the currently selected menu item.
} Menu;

/**
 * @brief Initializes the menu with the provided items and number of items.
 * @param items Pointer to an array of MenuItem structures representing the menu options.
 * @param num_items The number of items in the menu (and in items).
 */
void menu_init(MenuItem* items, i32 num_items);

#endif // GRAPHICS_MENU_H
