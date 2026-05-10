#include "app/App.h"

App::App(Font font) {
    this->font = font;

    for (int i = 0; i < 150; i++) {
        int size = GetRandomValue(20, 40);
        int pos_x = GetRandomValue(20, GetScreenWidth() - 20);
        int pos_y = GetRandomValue(20, GetScreenHeight() - 20);

        Vector2 position = { pos_x, pos_y };
        cols.push_back(GlyphColumn{ size, position, font });
    }
}

void App::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    gui.draw();

    for (auto it = cols.begin(); it != cols.end(); it++) {
        it->update(gui);
        it->draw();
    }
    if (gui.is_size_updated()) { gui.update_size(); }
    image_input.draw(gui.is_keep_active(), gui.get_head_region());

    DrawFPS(10, 10);
    EndDrawing();
}

void App::update() {
    gui.update(image_input);
    update_cols();

    if (gui.unload_cliked()) { image_input.unload(); }
    if (gui.show_clicked())  { image_input.show(); }
    if (gui.hide_clicked())  { image_input.hide(); }
}

void App::update_cols() {
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
}

