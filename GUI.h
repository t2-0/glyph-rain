#pragma once
#include "raylib_raygui.h"
#include "ImageInput.h"
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

class Gui {
public:
	Gui();

	void draw();
	void update(ImageInput& image_input);

	float get_column_speed() const { return column_speed->get_val(); }
	float get_glyph_speed()  const { return glyph_speed->get_val(); }
	float get_head_region()  const { return head_region->get_val(); }
	int get_min_col_size()   const { return column_size->get_min(); }
	int get_max_col_size()   const { return column_size->get_max(); }
	float get_cols_amount()  const { return column_amount->get_val(); }
	bool is_keep_active () const { return keep_colors->is_active(); }

	bool is_size_updated() const { return column_size->is_updated(); }
	void update_size() { column_size->update(); }

	bool show_clicked() { bool b = show_btn->clicked(); show_btn->click(); return b; }
	bool hide_clicked() { bool b = hide_btn->clicked(); hide_btn->click(); return b; }
	bool load_clicked() { bool b = load_btn->clicked(); load_btn->click(); return b; }
	bool unload_cliked() { bool b = unload_btn->clicked(); unload_btn->click(); return b; }
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
	unique_ptr<ASCIIPreview> ascii_preview;

	unique_ptr<Button> load_btn;
	unique_ptr<Button> unload_btn;
	unique_ptr<Button> show_btn;
	unique_ptr<Button> hide_btn;
	unique_ptr<Toggle> keep_colors;
	unique_ptr<TextEx> keep_label;

	unique_ptr <ColorGroup> colors;

	vector<vector<StyleEntry>> style_colors;
};