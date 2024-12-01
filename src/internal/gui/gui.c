#include <internal/GUI/gui.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <external/raygui.h>

void gui_menu(void) {
    unsigned char showMessageBox;
    showMessageBox = 0;
    InitWindow(640, 320,
               "Banca de Código da Universidade do Estado do Rio de Janeiro");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        if (GuiButton((Rectangle){24, 24, 120, 30}, "#191#Show Message")) {
            showMessageBox = 1;
        }

        if (showMessageBox != 0) {
            int result =
                GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box",
                              "Hi! This is a message!", "Nice;Cool");

            if (result >= 0) {
                showMessageBox = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
}