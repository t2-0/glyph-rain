#pragma once
#include "raylib_raygui.h"
#include "gui_elements.h"
#include <memory>
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

class GlyphImagePreview {
public:
	GlyphImagePreview(Rectangle bounds);

	void draw();
	void update_points();
	void clear_points() { points.clear(); file_name.clear(); }

	string get_file_name() { return file_name; }
private:
	Rectangle bounds;
	string file_name;

	Vector2 position;
	Vector2 size;
	vector<Vector2> points;
};

class Gui {
public:
	Gui();

	void draw();
	void update();

	float get_column_speed() const { return column_speed->get_val(); }
	float get_glyph_speed()  const { return glyph_speed->get_val(); }
	float get_head_region()  const { return head_region->get_val(); }
	int get_min_col_size() const { return column_size->get_min(); }
	int get_max_col_size() const { return column_size->get_max(); }
	float get_cols_amount()  const { return column_amount->get_val(); }

	string get_input()  { return input->get_text(); }
	string get_file_name() { return image_preview->get_file_name(); }
	bool input_active() { return input->is_active(); }

	bool is_size_updated() const { return column_size->is_updated(); }
	void update_size() { column_size->update(); }

	bool fill_clicked()  { return fill_btn->clicked(); }
	bool clear_clicked() { return clear_btn->clicked(); }
private:
	unique_ptr<Panel> panel;

	unique_ptr<TextEx> column_speed_text;
	unique_ptr<Slider<float>> column_speed;

	unique_ptr<TextEx> glyph_text;
	unique_ptr<Slider<float>> glyph_speed;

	unique_ptr<TextEx> head_text;
	unique_ptr<Slider<float>> head_region;

	unique_ptr<TextEx> column_size_text;
	unique_ptr<RangeSlider> column_size;

	unique_ptr<TextEx> column_amount_text;
	unique_ptr<Slider<int>> column_amount;

	unique_ptr<TextEx>  input_text;
	unique_ptr<TextBox> input;
	unique_ptr<GlyphImagePreview> image_preview;

	unique_ptr<Button> load_btn;
	unique_ptr<Button> remove_btn;
	unique_ptr<Button> fill_btn;
	unique_ptr<Button> clear_btn;

	unique_ptr <ColorGroup> colors;

	vector<vector<StyleEntry>> style_colors;
};