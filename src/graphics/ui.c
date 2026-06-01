#include "ui.h"

#include "core/config.h"
#include "main_menu.h"

#include "raylib.h"

#include <unistd.h>

void ui_init(UI* ui) {
    ui->state              = UI_STATE_MAIN_MENU;
    ui->selected_menu_item = 0;
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Escape Puzzle");
    SetWindowPosition(0, 0);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (ui->state) {
        case UI_STATE_MAIN_MENU:
            update_main_menu(ui);
            draw_main_menu(ui);
            break;
        case UI_STATE_IN_GAME:

            break;
        case UI_STATE_PAUSE:
            break;
        case UI_STATE_GAME_OVER:
            break;
        }
        EndDrawing();
    }
}
