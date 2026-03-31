#include "GUI.h"

GUI::GUI() {
	float screen_width = GetScreenWidth();
	float screen_height = GetScreenHeight();

	float panel_width = 220.0f;
	panel = make_unique<Panel>(Rectangle{ screen_width - panel_width, 0, panel_width, screen_height }, nullptr);

	Font font = GetFontDefault();// t
	string text_c = "Column Speed";
	Color text_color = GetColor(GuiGetStyle(LABEL, TEXT_COLOR_NORMAL));
	float text_c_width = MeasureTextEx(font, text_c.c_str(), font.baseSize, 1.0f).x;

	float slider_width = 120.0f;
	float slider_x = screen_width - (slider_width + (panel_width - slider_width) / 2.0f);
	float text_c_x = round(slider_x + slider_width / 2.0f - text_c_width / 2.0f);

	column_text = make_unique<TextEx>(Vector2{ text_c_x, 30 }, text_c, text_color);
	column_speed = make_unique<Slider>(Rectangle{ slider_x, 50, slider_width, 20 }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);

	string text_g = "Glyph Speed";
	float text_g_width = MeasureTextEx(font, text_c.c_str(), font.baseSize, 1.0f).x;
	float text_g_x = round(slider_x + slider_width / 2.0f - text_g_width / 2.0f);


	glyph_text = make_unique<TextEx>(Vector2{ text_g_x, 90 }, text_g.c_str(), text_color);
	glyph_speed = make_unique<Slider>(Rectangle{ slider_x, 110, 120, 20 }, "0.0", "2.0", 1.0f, 0.0f, 2.0f);
}

void GUI::draw() {
	panel.get()->draw();

	column_text.get()->draw();
	column_speed.get()->draw();

	glyph_text.get()->draw();
	glyph_speed.get()->draw();
}