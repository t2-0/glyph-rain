#include "raylib_raygui.h"
#include "Glyphs.h"
#include "GUI.h"
#include "ImageInput.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(1280, 1024, "Glyph Rain");
    string file = "styles/terminal/style_terminal.rgs";
    GuiLoadStyle(file.c_str());
    
    Font font = GetFontDefault();
    Glyph::set_font(font);
    TextEx::set_font(GuiGetFont());
    ASCIIRenderer::set_step(8);

    vector<GlyphColumn> cols;
    for (int i = 0; i < 150; i++) {
        int size = GetRandomValue(20, 40);
        int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
        int pos_y = GetRandomValue(20, GetScreenHeight() - 20);

        Vector2 position = { pos_x, pos_y };
        cols.push_back(GlyphColumn{ size, position, font });
    }

    Gui gui;
    ImageInput input_image = { { 40.0f, 25.0f }, 980, 980 };

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

                if (cols.size() <= 8) { to_del = 1; };
                if (it->out_of_height() && to_del >= 1) {
                    it = cols.erase(it);
                }
                else { it++; }
            }
        }

        if (gui.load_clicked())  { input_image.load(gui.get_file_name()); }
        if (gui.unload_cliked()) { input_image.unload(); }
        if (gui.show_clicked())  { input_image.show(); }
        if (gui.hide_clicked())  { input_image.hide(); }

        BeginDrawing();
        ClearBackground(BLACK);

        gui.draw();

        for (auto it = cols.begin(); it != cols.end(); it++) {
            it->update(gui);
            it->draw();
        }
        if (gui.is_size_updated()) { gui.update_size(); }
        input_image.draw(gui.is_keep_active(), gui.get_head_region());

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}