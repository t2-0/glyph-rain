#include "raylib_raygui.h"
#include "Glyphs.h"
#include "GUI.h"
#include "utils.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(1024, 768, "Glyph Rain");
    string file = "styles/terminal/style_terminal.rgs";
    GuiLoadStyle(file.c_str());
    
    Font font = GetFontDefault();
    Glyph::set_font(font);
    TextEx::set_font(GuiGetFont());

    vector<GlyphColumn> cols;
    for (int i = 0; i < 150; i++) {
        int size = GetRandomValue(20, 40);
        int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
        int pos_y = GetRandomValue(20, GetScreenHeight() - 20);

        Vector2 position = { pos_x, pos_y };
        cols.push_back(GlyphColumn{ size, position, font });
    }

    Gui gui;

    int font_size = 80;
    unique_ptr<Font> merged_font = merge_noto_fonts(font_size);
    GlyphInput input_text = { *merged_font, font_size };

    Vector2 image_size = { 500.0f, 500.0f };
    Vector2 image_pos = { ((GetScreenWidth() - 220.0f - image_size.x) / 2.0f), ((GetScreenHeight() - image_size.y) / 2.0f) };
    
    GlyphInput input_image = { image_size };
    input_image.set_position(image_pos);

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

        if (gui.fill_clicked()) {
           string input_t = gui.get_input();
           string file_name = gui.get_file_name();

           Vector2 text_size  = MeasureTextEx(*merged_font, input_t.c_str(), font_size, 1.0f);
           Vector2   text_pos = { ((GetScreenWidth() - 220.0f - text_size.x) / 2.0f), ((GetScreenHeight() - text_size.y) / 2.0f) };

           if (!file_name.empty()) {
               text_pos.y = image_pos.y - text_size.y - 5.0f;
               input_image.update_texture(file_name);
           }
           else { input_image.clear_texture(); }

           input_text.set_position(text_pos);
           input_text.update_texture(input_t);
        }

        if (gui.clear_clicked()) {
            input_text.clear_texture();
            input_image.clear_texture();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        gui.draw();

        for (auto it = cols.begin(); it != cols.end(); it++) {
            it->update(gui);
            it->draw();

            input_text.update(*it);
            input_image.update(*it);
        }
        if (gui.is_size_updated()) { gui.update_size(); }

        input_text.draw(gui);
        input_image.draw(gui);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}