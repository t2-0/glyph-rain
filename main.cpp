#include "raylib_raygui.h"
#include "GUI.h"
#include "Glyphs.h"

// function from example
void AddCodepointRange(Font* font, const char* fontPath, int start, int stop) {
    int rangeSize = stop - start + 1;
    int currentRangeSize = font->glyphCount;

    // TODO: Load glyphs from provided vector font (if available),
    // add them to existing font, regenerating font image and texture

    int updatedCodepointCount = currentRangeSize + rangeSize;
    int* updatedCodepoints = (int*)RL_CALLOC(updatedCodepointCount, sizeof(int));

    // Get current codepoint list
    for (int i = 0; i < currentRangeSize; i++) updatedCodepoints[i] = font->glyphs[i].value;

    // Add new codepoints to list (provided range)
    for (int i = currentRangeSize; i < updatedCodepointCount; i++)
        updatedCodepoints[i] = start + (i - currentRangeSize);

    UnloadFont(*font);
    *font = LoadFontEx(fontPath, 32, updatedCodepoints, updatedCodepointCount);
    RL_FREE(updatedCodepoints);
}

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(1024, 768, "Glyph Rain");
    string file = "styles/terminal/style_terminal.rgs";
    GuiLoadStyle(file.c_str());

    //Font font = LoadFont("NotoSansJP-Regular.ttf"); NotoSansJP ?
    Font font = GetFontDefault();
    /* 
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x4e00, 0x9fff);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x3400, 0x4dbf);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x3000, 0x303f);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x3040, 0x309f);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x30A0, 0x30ff);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x31f0, 0x31ff);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0xff00, 0xffef);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0xac00, 0xd7af);
    AddCodepointRange(&font, "NotoSansJP-Regular.ttf", 0x1100, 0x11ff);
    */
    Glyph::set_font(font);
    TextEx::set_font(GuiGetFont());

    vector<GlyphColumn> cols;
    for (int i = 0; i < 100; i++) {
        int size = GetRandomValue(20, 40);
        int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
        int pos_y = GetRandomValue(20, GetScreenHeight() - 20);

        Vector2 position = { pos_x, pos_y };
        cols.push_back(GlyphColumn{ size, position, font });
    }
    GUI gui;

    SetTargetFPS(60);
    float* val = new float(1.0f);
    while (!WindowShouldClose()) {
        gui.update();
        BeginDrawing();
        ClearBackground(BLACK);

        gui.draw();
        for (int i = 0; i < cols.size(); i++) {
            cols[i].update(gui);
            cols[i].draw();
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}