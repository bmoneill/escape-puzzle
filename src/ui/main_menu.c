#include "main_menu.h"

#include "core/config.h"
#include "ui.h"

#include "raylib.h"

void update_main_menu(UI* ui) {
    if (IsKeyPressed(KEY_DOWN)) {
        ui->selected_menu_item = (ui->selected_menu_item + 1) % 3;
    } else if (IsKeyPressed(KEY_UP)) {
        ui->selected_menu_item = (ui->selected_menu_item - 1 + 3) % 3;
    } else if (IsKeyPressed(KEY_ENTER)) {
        switch (ui->selected_menu_item) {
        case 0: // Start Game
            ui->state = UI_STATE_IN_GAME;
            break;
        case 1: // Options
            break;
        }
    }
}

void draw_main_menu(UI* ui) {
    const char* menu_items[] = { "Start Game", "Options", "Exit" };
    const int   num_items    = sizeof(menu_items) / sizeof(menu_items[0]);

    for (int i = 0; i < num_items; i++) {
        Color color = (i == selected_item) ? RED : WHITE;
        DrawText(menu_items[i], 100, 100 + i * 40, 20, color);
    }
}
