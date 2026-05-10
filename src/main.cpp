#include "app/App.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
#endif

void update_draw(void* arg) {
    App* app = (App*)arg;

    app->update();
    app->draw();
}

int main() {
    InitWindow(896, 672, "Glyph Rain");

    GuiLoadStyleTerminalW();

    Font font = GetFontDefault();
    Glyph::set_font(font);
    TextEx::set_font(GuiGetFont());
    ASCIIRenderer::set_step(8);

    App app = { font };

    SetTargetFPS(60);
    bool undecorated = false;
    #ifdef _WIN32
        while (!WindowShouldClose()) {
            if (IsKeyPressed(KEY_TAB)) {
                undecorated = !undecorated;
                if (undecorated) {
                    SetWindowState(FLAG_WINDOW_UNDECORATED);
                }
                else {
                    ClearWindowState(FLAG_WINDOW_UNDECORATED);
                }
            }

            update_draw(&app);
        }
    #elif __EMSCRIPTEN__
        emscripten_set_main_loop_arg(update_draw, &app, 0, 1);
    #endif 

    UnloadFont(font);
    CloseWindow();
    return 0;
}