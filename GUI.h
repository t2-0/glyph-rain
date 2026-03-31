#pragma once
#include <memory>
#include "raylib_raygui.h"
#include "gui_elements.h"
#include <iostream>

using namespace std;

using std::unique_ptr;
using std::make_unique;

class GUI {
public:
	GUI();

	void draw();

	float get_column_speed() const { return column_speed.get()->get_val(); }
	float get_glyph_speed()  const { return glyph_speed.get()->get_val(); }
	float get_head_region()  const { return head_region.get()->get_val(); }
private:
	unique_ptr<Panel> panel;

	unique_ptr<TextEx> column_text;
	unique_ptr<Slider> column_speed;

	unique_ptr<TextEx> glyph_text;
	unique_ptr<Slider> glyph_speed;

	unique_ptr<TextEx> head_text;
	unique_ptr<Slider> head_region;
};