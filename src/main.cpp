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
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(896, 672, "Glyph Rain");

    GuiLoadStyleTerminalW();

    Font font = GetFontDefault();
    Glyph::set_font(font);
    TextEx::set_font(GuiGetFont());
    ASCIIRenderer::set_step(8);

    App app = { font };

    SetTargetFPS(60);
    #ifdef _WIN32
        while (!WindowShouldClose()) { update_draw(&app); }
    #elif __EMSCRIPTEN__
        emscripten_set_main_loop_arg(update_draw, &app, 0, 1);
    #endif 

    UnloadFont(font);
    CloseWindow();
    return 0;
}