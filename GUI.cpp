#include "GUI.h"

float get_text_x(float slider_x, float slider_width, string text) {
	Font font = GetFontDefault();// t
	float text_width = MeasureTextEx(font, text.c_str(), font.baseSize, 1.0f).x;
	return round(slider_x + slider_width / 2.0f - text_width / 2.0f);
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
}

void GUI::draw() {
	panel.get()->draw();

	column_text.get()->draw();
	column_speed.get()->draw();

	glyph_text.get()->draw();
	glyph_speed.get()->draw();

	head_text.get()->draw();
	head_region.get()->draw();
}