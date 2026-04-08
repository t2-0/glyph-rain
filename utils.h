#pragma once
#include "raylib_raygui.h"
#include <memory>

using std::unique_ptr;
using std::make_unique;

unique_ptr<Font> merge_noto_fonts(int font_size);