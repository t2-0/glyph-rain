#pragma once
#include <memory>
#include "raylib_raygui.h"
#include "gui_elements.h"
#include <iostream>

using namespace std;

using std::unique_ptr;
using std::make_unique;

struct StyleEntry {
	StyleEntry(int control, int property, Color color) : control{ control }, property{ property }, color{ color } { }

	int control;
	int property;
	Color color;
};

class GUI {
public:
	GUI();

	void draw();
	void update();

	float get_column_speed() const { return column_speed->get_val(); }
	float get_glyph_speed()  const { return glyph_speed->get_val(); }
	float get_head_region()  const { return head_region->get_val(); }
	float get_min_col_size() const { return column_size->get_min(); }
	float get_max_col_size() const { return column_size->get_max(); }
	float get_cols_amount()  const { return column_amount->get_val(); }

	bool is_size_updated() const { return column_size->is_updated(); }
	void update_size() { column_size->update(); }
private:
	unique_ptr<Panel> panel;

	unique_ptr<TextEx> column_speed_text;
	unique_ptr<Slider> column_speed;

	unique_ptr<TextEx> glyph_text;
	unique_ptr<Slider> glyph_speed;

	unique_ptr<TextEx> head_text;
	unique_ptr<Slider> head_region;

	unique_ptr<TextEx> column_size_text;
	unique_ptr<RangeSlider> column_size;

	unique_ptr<TextEx> column_amount_text;
	unique_ptr<Slider> column_amount;

	unique_ptr <ColorGroup> colors;

	vector<vector<StyleEntry>> style_colors;
};