#include "Gui.h"
#include "win32_dialog.h"

int get_text_x(float slider_x, float slider_width, string text) {
	Font font = TextEx::get_font();
	float text_width = MeasureTextEx(font, text.c_str(), font.baseSize, 0.0f).x;
	return int(slider_x + slider_width / 2.0f - text_width / 2.0f);
}

Color get_styled_color(Color style_color, Color color) {
	Vector3 style_hsv = ColorToHSV(style_color);
	Vector3 in_hsv	  = ColorToHSV(color);

	Color color_custom;
	// for Color like GRAY, WHITE, BLACK
	if (in_hsv.y < 0.01f) {
		color_custom = ColorFromHSV(style_hsv.x, 0.0f, in_hsv.z * style_hsv.z);
	}
	else {
		color_custom = ColorFromHSV(in_hsv.x, style_hsv.y, style_hsv.z);
	}

	return color_custom;
}

Gui::Gui() {
	float screen_width = GetScreenWidth();
	float screen_height = GetScreenHeight();

	float panel_width = 220.0f;
	panel = make_unique<Panel>(Rectangle{ screen_width - panel_width, 0.0f, panel_width, screen_height }, nullptr);

	string text = "Column Speed";
	Color text_color = GetColor(GuiGetStyle(LABEL, TEXT_COLOR_NORMAL));

	const float slider_width = 120.0f;
	const float slider_height = 18.0f;
	float slider_x = screen_width - (slider_width + (panel_width - slider_width) / 2.0f);
	float text_x = get_text_x(slider_x, slider_width, text);

	column_speed_text = make_unique<TextEx>(Vector2{ text_x, 30.0f }, text, text_color);
	column_speed = make_unique<Slider<float>>(Rectangle{ slider_x, 50.0f, slider_width, slider_height }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);

	text = "Glyph Speed";
	text_x = get_text_x(slider_x, slider_width, text);

	glyph_text = make_unique<TextEx>(Vector2{ text_x, 90.0f }, text.c_str(), text_color);
	glyph_speed = make_unique<Slider<float>>(Rectangle{ slider_x, 110.0f, slider_width, slider_height }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);

	text = "Head Region";
	text_x = get_text_x(slider_x, slider_width, text);

	head_text = make_unique<TextEx>(Vector2{ text_x, 150.0f }, text.c_str(), text_color);
	head_region = make_unique<Slider<float>>(Rectangle{ slider_x, 170.0f, slider_width, slider_height }, "0.0", "1.0", 0.5f, 0.0f, 1.0f);

	text = "Amount of Columns";
	text_x = get_text_x(slider_x, slider_width, text);

	column_amount_text = make_unique<TextEx>(Vector2{ text_x, 210.0f }, text.c_str(), text_color);
	column_amount = make_unique<Slider<int>>(Rectangle{ slider_x, 230.0f, slider_width, slider_height }, "0", "200", 100, 0, 200);

	text = "Column Size";
	text_x = get_text_x(slider_x, slider_width, text);

	column_size_text = make_unique<TextEx>(Vector2{ text_x, 270.0f }, text.c_str(), text_color);
	column_size = make_unique<RangeSlider>(Rectangle{ slider_x, 290.0f, slider_width, slider_height }, "0", "100", 0, 100);
	
	text = "Input";
	text_x = get_text_x(slider_x, slider_width, text);

	input_text = make_unique<TextEx>(Vector2{ text_x, 365.0f }, text.c_str(), text_color);
	ascii_preview = make_unique<ASCIIPreview>(Rectangle{ slider_x - 20.0f, 388.0f, 160.0f, 160.0f }, Vector2{ slider_x - 10.0f, 396.0f }, 140, 140);

	keep_colors = make_unique<Toggle>(Rectangle{ slider_x + 115.0f, 555.0f, 25.0f, 25.0f }, "");
	keep_label = make_unique<TextEx>(Vector2{ slider_x - 20.0f, 558.0f }, "Keep colors", text_color);
	load_btn = make_unique<Button>(Rectangle{ slider_x - 20.0f, 585.0f, 70.0f, 25.0f }, "Load");
	unload_btn = make_unique<Button>(Rectangle{ slider_x - 20.0f,615.0f, 70.0f,25.0f }, "Unload");
	show_btn = make_unique<Button>(Rectangle{ slider_x + 70.0f, 585.0f, 70.0f, 25.0f }, "Show");
	hide_btn = make_unique<Button>(Rectangle{ slider_x + 70.0f, 615.0f, 70.0f, 25.0f }, "Hide");

	Color style_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	vector<Color> colors_v;
	vector<Color> colors_orig = { style_color, RED, BLUE, YELLOW, PURPLE, RAYWHITE };
	for (int i = 0; i < colors_orig.size(); i++) {
		Color color_custom = get_styled_color(style_color, colors_orig[i]);
		colors_v.push_back(color_custom);
	}
	colors = make_unique<ColorGroup>(Rectangle{ slider_x, screen_height * 0.86f, 30.0f, 30.0f }, 15.0f, 10.0f, colors_v, 3, 1);
	
	vector<pair<int, int>> style_pairs = { 
		{ DEFAULT, TEXT_COLOR_NORMAL },
		{ SLIDER, BORDER_COLOR_NORMAL },
		{ SLIDER, BASE_COLOR_PRESSED },
		{ SLIDER, BORDER_COLOR_PRESSED },
		{ SLIDER, TEXT_COLOR_PRESSED },
		{ LABEL, TEXT_COLOR_PRESSED },
		{ TOGGLE, BORDER_COLOR_NORMAL },
		{ TOGGLE, BORDER_COLOR_PRESSED },
		{ TOGGLE, BASE_COLOR_PRESSED },
		{ TOGGLE, BASE_COLOR_FOCUSED },
		{ DEFAULT, BACKGROUND_COLOR },
		{ TEXTBOX, BORDER_COLOR_NORMAL },
		{ TEXTBOX, BORDER_COLOR_PRESSED },
		{ TEXTBOX, BASE_COLOR_PRESSED },
		{ TEXTBOX, BASE_COLOR_NORMAL },
		{ TEXTBOX, BASE_COLOR_PRESSED },
		{ BUTTON, BASE_COLOR_FOCUSED },
		{ BUTTON, BASE_COLOR_PRESSED },
		{ BUTTON, BORDER_COLOR_NORMAL},
		{ BUTTON, BORDER_COLOR_PRESSED },
		{ BUTTON, TEXT_COLOR_PRESSED }
};

	for (int i = 0; i < colors_v.size(); i++) {
		style_colors.push_back({});

		for (int r = 0; r < style_pairs.size(); r++) {
			int control = style_pairs[r].first;
			int property = style_pairs[r].second;

			style_color = GetColor(GuiGetStyle(control, property));
			style_color = get_styled_color(style_color, colors_v[i]);
			style_colors.back().push_back({ control, property, style_color });
		}
	}
}

void Gui::draw() {
	panel->draw();

	column_speed_text->draw();
	column_speed->draw();

	glyph_text->draw();
	glyph_speed->draw();

	head_text->draw();
	head_region->draw();

	column_size_text->draw();
	column_size->draw();

	column_amount_text->draw();
	column_amount->draw();

	input_text->draw();
	ascii_preview->draw(is_keep_active(), get_head_region());

	load_btn->draw();
	unload_btn->draw();
	show_btn->draw();
	hide_btn->draw();

	keep_colors->draw();
	keep_label->draw();

	colors->draw();
}

void Gui::update() {
	if (colors->is_updated()) {
		colors->update();

		Color color_active = colors->get_active_color();
		int active = colors->get_active();
		
		for (int i = 0; i < style_colors[active].size(); i++) {
			GuiSetStyle(style_colors[active][i].control, style_colors[active][i].property, ColorToInt(style_colors[active][i].color));
		}

		column_speed_text->set_color(color_active);
		glyph_text->set_color(color_active);
		head_text->set_color(color_active);
		column_amount_text->set_color(color_active);
		column_size_text->set_color(color_active);
		column_size->set_text_color(color_active);
		input_text->set_color(color_active);
		keep_label->set_color(color_active);
	}

	if (load_btn->clicked())   {
		HWND hwnd = (HWND)GetWindowHandle();
		ascii_preview->extract(load_file(hwnd));
	}
	if (unload_btn->clicked()) { ascii_preview->clear(); }

	if (column_speed->is_updated()) { column_speed->update(); }
	if (glyph_speed->is_updated())  { glyph_speed->update(); }
	if (head_region->is_updated())  { head_region->update(); }
	if (column_amount->is_updated()) { column_amount->update(); }
	if (keep_colors->is_updated()) { keep_colors->update(); }
}