#include "gui_elements.h"
#include <iostream>

using namespace std;

Font TextEx::font;

IntBox::IntBox(Rectangle bounds, string text, int min_val, int max_val, int val = 0) :
	bounds{ bounds }, min_val{ min_val }, max_val{ max_val }, value{ val } {
	this->text = new char[text.size()] {};
	strcpy(this->text, text.c_str());
}

void IntBox::draw() {
	if (value > max_val) {
		value = max_val;
	}
	else if (value < min_val) {
		value = min_val;
	}

	if (GuiValueBox(bounds, text, &value, min_val, max_val, edit_mode)) {
		edit_mode = !edit_mode;
	}
}

void TimedText::draw() {
	if (triggered) {
		float delta_time = GetFrameTime();
		timer += delta_time;

		if (timer >= lifetime) {
			timer = 0.0f;
			triggered = false;
		}

		text.draw();
	}
}

ScrollBar::ScrollBar(Rectangle bounds, float min_scroll, float font_size, size_t item_count) {
	this->bounds = bounds;
	this->min_scroll = min_scroll;
	this->max_scroll = 10 + item_count * (font_size + 10);

	scroll_val = 0;
}

void ScrollBar::draw() {
	scroll_val = GuiScrollBarW(bounds, scroll_val, min_scroll, max_scroll);
}

ColorGroup::ColorGroup(Rectangle toggle_bounds, float offset_x, float offset_y, vector<Color> colors, int toggles_per_row, int active) {
	this->active = active;
	active_new   = active;
	float base_x = toggle_bounds.x;

	if (toggles_per_row <= 0) { toggles_per_row = 1; }
	for (int i = 0; i < colors.size(); i++) {
		toggles.push_back({ toggle_bounds, colors[i] });

		toggle_bounds.x += toggle_bounds.width + offset_x;
		if ((i + 1) % toggles_per_row == 0) {
			toggle_bounds.y += toggle_bounds.height + offset_y;
			toggle_bounds.x = base_x;
		}
	}

	if (!toggles.empty()) { toggles[active].set_active(true); }
}

void ColorGroup::update() {
	toggles[active].set_active(false);
	toggles[active].update();

	active = active_new;
	toggles[active].set_active(true);
	toggles[active].update();
}

bool ColorGroup::is_updated() {
	bool updated = false;
	for (int i = 0; i < toggles.size(); i++) {
		if (toggles[i].is_updated()) {
			active_new = i;
			updated = true;
			break;
		}
	}

	return updated;
}

void Panel::draw() {
	Color color_base = Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f);
	DrawRectangleRec(bounds, color_base);
	DrawRectangleLinesEx(bounds, 1.0f, WHITE);
}

RangeSlider::RangeSlider(Rectangle bounds, string text_left, string text_right, int min_val_range, int max_val_range) {
	string text = "[ ";
	text += to_string(min_val_range);
	text += " ... ";
	text += to_string(max_val_range);
	text += " ]";

	bounds_min = bounds;
	bounds.y += bounds.height + bounds.height / 2.0f;

	bounds_max = bounds;

	Font font = TextEx::get_font();
	Vector2 text_size = MeasureTextEx(font, text.c_str(), font.baseSize, 0.0f);

	Vector2 text_pos = { bounds.x + text_size.x / 2.0f, bounds.y + bounds.height };
	range = new TextEx{ text_pos, text, WHITE };

	this->text_left = text_left;
	this->text_right = text_right;
	this->min_val_range = min_val_range;
	this->max_val_range = max_val_range;

	min_val = min_val_range;
	max_val = max_val_range;

	old_min = this->min_val;
	old_max = this->max_val;
}

void RangeSlider::draw() {
	if (old_min != min_val) {
		updated = true;
		if (max_val < min_val) { max_val = min_val; }

		adjust_text(min_val, max_val);
		old_min = min_val;
	}

	if (old_max != max_val) {
		updated = true;
		if (min_val > max_val) { min_val = max_val; }

		adjust_text(min_val, max_val);
		old_max = max_val;
	}

	float min_valf = min_val;
	GuiSlider(bounds_min, text_left.c_str(), text_right.c_str(), &min_valf, min_val_range, max_val_range);
	min_val = min_valf;

	float max_valf = max_val;
	GuiSlider(bounds_max, text_left.c_str(), text_right.c_str(), &max_valf, min_val_range, max_val_range);
	max_val = max_valf;
	range->draw();
}

void RangeSlider::adjust_text(int min_val, int max_val) {
	string text = "[ ";
	text += to_string(min_val);
	text += " ... ";
	text += to_string(max_val);
	text += " ]";

	range->set_text(text);
}

void TextBox::draw() {
	Color border_color = GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_NORMAL));
	Color bg_color     = GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_NORMAL));

	Vector2 mouse_pos = GetMousePosition();
	if (CheckCollisionPointRec(mouse_pos, bounds)) {
		border_color = GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_FOCUSED));
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { edit_mode = !edit_mode; }
	}
	else {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { edit_mode = false; }
	}

	if (edit_mode) {
		border_color = GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED));
		bg_color	 = GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED));

		int key = GetCharPressed();
		if (key >= 32) {
			int utf8Size = 0;
			const char* utf8 = CodepointToUTF8(key, &utf8Size);

			int text_width = MeasureTextEx(font, text.c_str(), 16.0f, 0.0f).x + 6.0f;
			int char_width = MeasureTextEx(font, utf8, 16.0f, 0.0f).x;

			text_width += char_width;
			if (text_width < bounds.width) {

				text += utf8;
				key = GetCharPressed();
			}
		}

		if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
			size_t len = text.size();
			while (len > 0 && ((text[len - 1] & 0xC0) == 0x80)) {
				len--;
			}

			if (len > 0) { len--; } // remove leading bit
			text.resize(len);
		}

		if (IsKeyPressed(KEY_ENTER)) { edit_mode = false; }
	}

	DrawRectangleRec(bounds, bg_color);
	DrawRectangleLinesEx(bounds, 1.0f, border_color);

	int text_width = MeasureTextEx(font, text.c_str(), 16.0f, 0.0f).x;

	Vector2 line_start = { bounds.x + text_width + 6.0f, bounds.y + 3.0f };
	Vector2 line_end   = { bounds.x + text_width + 6.0f, bounds.y + bounds.height - 3.0f };

	if (edit_mode) { DrawLineEx(line_start, line_end, 2.0f, border_color); }

	Vector2 text_pos = { bounds.x + 5.0f,  bounds.y + 3.0f };
	DrawTextEx(font, text.c_str(), text_pos, 16.0f, 0.0f, border_color);
}

template<>
void Slider<float>::draw() {
	GuiSlider(bounds, text_left.c_str(), text_right.c_str(), &val, min_val, max_val);

	Font font = GuiGetFont();
	Color color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	DrawTextEx(font, val_str.c_str(), val_pos, font.baseSize, 0.0f, color);
}

template<>
void Slider<int>::draw() {
	float val_f = val;
	GuiSlider(bounds, text_left.c_str(), text_right.c_str(), &val_f, min_val, max_val);
	val = val_f;

	Font font = GuiGetFont();
	Color color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	DrawTextEx(font, val_str.c_str(), val_pos, font.baseSize, 0.0f, color);
}

template<>
void Slider<float>::update() {
	Font font = GuiGetFont();

	val_str = "[ ";
	val_str += to_string(val);
	val_str.erase(val_str.end() - 4, val_str.end());
	val_str += " ]";

	Vector2 text_size = MeasureTextEx(font, val_str.c_str(), font.baseSize, 0.0f);
	val_pos = { round(bounds.x + (bounds.width - text_size.x) / 2.0f), round(bounds.y + bounds.height) };

	old_val = val;
}

template<>
void Slider<int>::update() {
	Font font = GuiGetFont();

	val_str = "[ ";
	val_str += to_string(val);
	val_str += " ]";

	Vector2 text_size = MeasureTextEx(font, val_str.c_str(), font.baseSize, 0.0f);
	val_pos = { round(bounds.x + (bounds.width - text_size.x) / 2.0f), round(bounds.y + bounds.height) };

	old_val = val;
}