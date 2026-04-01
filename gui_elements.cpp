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

RangeSlider::RangeSlider(Rectangle bounds, string text_left, string text_right, float min_val, float max_val) {
	int min_val_i = min_val;
	int max_val_i = max_val;

	string text = "[ ";
	text += to_string(min_val_i);
	text += " ... ";
	text += to_string(max_val_i);
	text += " ]";

	Font font = TextEx::get_font();
	Vector2 text_size = MeasureTextEx(font, text.c_str(), font.baseSize, 0.0f);

	Vector2 text_pos = { int(bounds.x + text_size.x / 2.0f), bounds.y};
	range = new TextEx{ text_pos, text, WHITE };

	bounds.y += bounds.height + int(text_size.y / 4.0f);
	min_slider = new Slider{ bounds, text_left, text_right, min_val, min_val, max_val };

	bounds.y += bounds.height + bounds.height / 2.0f;
	max_slider = new Slider{ bounds, text_left, text_right, max_val, min_val, max_val };
}

RangeSlider::~RangeSlider() {
	delete range;
	delete min_slider;
	delete max_slider;
}

void RangeSlider::draw() {
	if (min_slider->is_updated()) {
		updated = true;
		int min_val = min_slider->get_val();
		int max_val = max_slider->get_val();

		if (max_val < min_val) {
			max_slider->set_val(min_val);
		}

		string text = "[ ";
		text += to_string(min_val);
		text += " ... ";
		text += to_string(max_val);
		text += " ]";
		range->set_text(text);

		min_slider->update();
	}

	if (max_slider->is_updated()) {
		updated = true;
		int min_val = min_slider->get_val();
		int max_val = max_slider->get_val();

		if (min_val > max_val) {
			min_slider->set_val(max_val);
		}

		string text = "[ ";
		text += to_string(min_val);
		text += " ... ";
		text += to_string(max_val);
		text += " ]";
		range->set_text(text);

		max_slider->update();
	}

	range->draw();
	min_slider->draw();
	max_slider->draw();
}
