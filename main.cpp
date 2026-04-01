#include "raylib_raygui.h"
#include "GUI.h"
#include "Glyphs.h"
#include <list>

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

    list<GlyphColumn> cols;
    for (int i = 0; i < 150; i++) {
        int size = GetRandomValue(20, 40);
        int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
        int pos_y = GetRandomValue(20, GetScreenHeight() - 20);

        Vector2 position = { pos_x, pos_y };
        cols.push_back(GlyphColumn{ size, position, font });
    }
    GUI gui;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        gui.update();
        int target_amount = gui.get_cols_amount();
        if (target_amount > cols.size()) {
            int size = GetRandomValue(gui.get_min_col_size(), gui.get_max_col_size());
            int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
            int pos_y = GetScreenHeight();

            Vector2 position = { pos_x, pos_y };
            cols.push_back(GlyphColumn{ size, position, font });
        }
        else if (target_amount < cols.size()) {
            for (auto it = cols.begin(); it != cols.end();) {
                // 0, 1 -> to_del == 66% 
                int to_del = GetRandomValue(0, 2);
                if (it->out_of_height() && to_del >= 1) {
                    it = cols.erase(it);
                }
                else {
                    it++;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        gui.draw();
        for (auto it = cols.begin(); it != cols.end(); it++) {
            it->update(gui);
            it->draw();
        }
        gui.update_size();

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}