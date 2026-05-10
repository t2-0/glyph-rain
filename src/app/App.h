#pragma once
#include "raylib_raygui.h"
#include "glyphs/Glyphs.h"
#include "image_input/ImageInput.h"

// Main application controller.
//
// - Introduced to simplify Web build flow
// - Removes need for ASYNCIFY-based control flow
// - Shared between desktop and web builds
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