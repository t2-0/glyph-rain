#include "GUI.h"

float get_text_x(float slider_x, float slider_width, string text) {
	Font font = GetFontDefault();// t
	float text_width = MeasureTextEx(font, text.c_str(), font.baseSize, 1.0f).x;
	return round(slider_x + slider_width / 2.0f - text_width / 2.0f);
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

GUI::GUI() {
	float screen_width = GetScreenWidth();
	float screen_height = GetScreenHeight();

	float panel_width = 220.0f;
	panel = make_unique<Panel>(Rectangle{ screen_width - panel_width, 0, panel_width, screen_height }, nullptr);

	string text = "Column Speed";
	Color text_color = GetColor(GuiGetStyle(LABEL, TEXT_COLOR_NORMAL));

	float slider_width = 120.0f;
	float slider_x = screen_width - (slider_width + (panel_width - slider_width) / 2.0f);
	float text_x = get_text_x(slider_x, slider_width, text);

	column_text = make_unique<TextEx>(Vector2{ text_x, 30 }, text, text_color);
	column_speed = make_unique<Slider>(Rectangle{ slider_x, 50, slider_width, 20 }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);

	text = "Glyph Speed";
	text_x = get_text_x(slider_x, slider_width, text);

	glyph_text = make_unique<TextEx>(Vector2{ text_x, 90 }, text.c_str(), text_color);
	glyph_speed = make_unique<Slider>(Rectangle{ slider_x, 110, 120, 20 }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);

	text = "Head Region";
	text_x = get_text_x(slider_x, slider_width, text);

	head_text = make_unique<TextEx>(Vector2{ text_x, 150 }, text.c_str(), text_color);
	head_region = make_unique<Slider>(Rectangle{ slider_x, 170, 120, 20 }, "0.0", "1.0", 0.5f, 0.0f, 1.0f);

	Color style_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	vector<Color> colors_v;
	vector<Color> colors_orig = { style_color, RED, BLUE, YELLOW, GRAY, PURPLE };
	for (int i = 0; i < colors_orig.size(); i++) {
		Color color_custom = get_styled_color(style_color, colors_orig[i]);
		colors_v.push_back(color_custom);
	}

	colors = make_unique<ColorGroup>(Rectangle{ slider_x, screen_height * 0.86f, 30.0f, 30.0f }, 15.0f, 10.0f, colors_v, 3, 1);
	
	for (int i = 0; i < colors_v.size(); i++) {
		style_colors.push_back({});
		style_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({DEFAULT, TEXT_COLOR_NORMAL, style_color});

	    style_color = GetColor(GuiGetStyle(SLIDER, BORDER_COLOR_NORMAL));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ SLIDER, BORDER_COLOR_NORMAL, style_color });

		style_color = GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ SLIDER, BASE_COLOR_PRESSED, style_color });

		style_color = GetColor(GuiGetStyle(SLIDER, BORDER_COLOR_PRESSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ SLIDER, BORDER_COLOR_PRESSED, style_color });

		style_color = GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_PRESSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ SLIDER, TEXT_COLOR_PRESSED, style_color });

		style_color = GetColor(GuiGetStyle(LABEL, TEXT_COLOR_PRESSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ LABEL, TEXT_COLOR_PRESSED, style_color });

		style_color = GetColor(GuiGetStyle(TOGGLE, BORDER_COLOR_NORMAL));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ TOGGLE, BORDER_COLOR_NORMAL, style_color });

		style_color = GetColor(GuiGetStyle(TOGGLE, BASE_COLOR_PRESSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ TOGGLE, BASE_COLOR_PRESSED, style_color });

		style_color = GetColor(GuiGetStyle(TOGGLE, BASE_COLOR_FOCUSED));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ TOGGLE, BASE_COLOR_FOCUSED, style_color });

		style_color = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
		style_color = get_styled_color(style_color, colors_v[i]);
		style_colors.back().push_back({ DEFAULT, BACKGROUND_COLOR, style_color });
	}
}

void GUI::draw() {
	panel.get()->draw();

	column_text.get()->draw();
	column_speed.get()->draw();

	glyph_text.get()->draw();
	glyph_speed.get()->draw();

	head_text.get()->draw();
	head_region.get()->draw();

	colors.get()->draw();
}

void GUI::update() {
	if (colors.get()->is_updated()) {
		colors.get()->update();
		Color color_active = colors.get()->get_active_color();
		int active = colors.get()->get_active();

		for (int i = 0; i < style_colors[active].size(); i++) {
			GuiSetStyle(style_colors[active][i].control, style_colors[active][i].property, ColorToInt(style_colors[active][i].color));
		}
		
		column_text.get()->set_color(color_active);
		glyph_text.get()->set_color(color_active);
		head_text.get()->set_color(color_active);
	}
}