#pragma once
#include "raylib_raygui.h"
#include "glyphs/Glyphs.h"
#include "image_input/ImageInput.h"

class App {
public:
    App(Font font);

    void draw();
    void update();
private:
    Gui gui;
    ImageInput image_input = { { 40.0f, 25.0f }, 620, 620 };
    vector<GlyphColumn> cols;
    Font font;

    void update_cols();
};