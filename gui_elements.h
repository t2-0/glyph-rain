#pragma once
#include "raylib_raygui.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class IntBox {
public:
	IntBox(Rectangle bounds, string text, int min_val, int max_val, int val);

	void draw();

	int get_val() { return value; }
private:
	Rectangle bounds;

	char* text = nullptr;
	int value = 0;
	int min_val = 0;
	int max_val = 0;
	bool edit_mode = false;
};

class Dropdownbox {
public:
	Dropdownbox(Rectangle bounds, string text) : bounds{ bounds }, text{ text } { }
	void draw() {if (GuiDropdownBox(bounds, text.c_str(), &active, edit_mode)) { edit_mode = !edit_mode; } }

	int get_active() { return active; }
	void set_active(int i) { active = i; }

	bool is_updated() { return active != old_active; }
	void update() { old_active = active; }

private:
	Rectangle bounds;
	string text;
	int active = 0;
	int old_active = 0;
	bool edit_mode = false;
};

class Button {
public:
	Button(Rectangle bounds, string text) : bounds{ bounds }, text{ text } {}

	void draw() { clicked_b = GuiButton(bounds, text.c_str()); }

	bool clicked() {
		if (clicked_b) {
			clicked_b = false;
			return true;
		}
		return false;
	}
private:
	Rectangle bounds = { };
	string text;

	bool clicked_b = false;
};

class Toggle {
public:
	Toggle(Rectangle bounds, string text) : bounds{ bounds }, text{ text } {}

	void draw() { if (GuiToggle(bounds, text.c_str(), &active)) { active = !active; } }
	bool is_active()  { return active; }
	bool is_updated() { return active != active_old; }
	void update() { active_old = active; }

	void set_active(bool active) { this->active = active; }
private:
	Rectangle bounds;
	string text;

	bool active = false;
	bool active_old = false;
};

class ToggleGroup {
public:
	ToggleGroup(Rectangle bounds, string text) : bounds{ bounds }, text{ text } {}

	void draw() { GuiToggleGroup(bounds, text.c_str(), &active); }
	bool is_updated() { return active != active_old; }
	void update() { active_old = active; }

	int get_active() { return active; }
	void set_active(int i) { active = i; }
private:
	Rectangle bounds;
	string text;
	int active = 0;
	int active_old = 0;
};

class Line {
public:
	Line() : start_pos{ 0 }, end_pos{ 0.0f }, thick{ 0.0f }, color{ RAYWHITE } {}
	Line(Vector2 start_pos, Vector2 end_pos, float thick, Color color) : start_pos{ start_pos }, end_pos{ end_pos },
																		 thick{ thick }, color{ color } { }

	void draw() { DrawLineEx(start_pos, end_pos, thick, color); }

	void set_start_pos(Vector2 start_pos) { this->start_pos = start_pos; }
	void set_end_pos(Vector2 end_pos) { this->end_pos = end_pos; }
	void set_thick(float thick) { this->thick = thick; }
	void set_color(Color color) { this->color = color; }
private:
	Vector2 start_pos;
	Vector2 end_pos;

	Color color;

	float thick;
};

class TextEx {
public:
	TextEx() { color = RAYWHITE; pos = { 0.0f, 0.0f }; }
	TextEx(Vector2 pos, string text, Color color) : pos{ pos }, text{ text }, color{ color } { }

	void draw() { DrawTextEx(font, text.c_str(), pos, font.baseSize, 0.0f, color); }

	void set_text(string text)  { this->text = text; }
	void set_pos(Vector2 pos)   { this->pos = pos; }
	void set_color(Color color) { this->color = color; }

	string get_text() const { return text; }
	Vector2 get_pos() { return pos; }

	static void set_font(Font font_n) { font = font_n; }
	static Font get_font() { return font; }
private:
	string text;
	Vector2 pos;

	static Font font;
	Color color;
};

class TimedText {
public:
	TimedText() { lifetime = 2; }

	void draw();
	void trigger() { triggered = true; }

	void set_text(TextEx text) { this->text = text; }
	void set_lifetime(float lifetime) { this->lifetime = lifetime; }
private:
	TextEx text;

	float lifetime = 0;
	float timer = 0;
	bool triggered = false;
};

class ScrollBar {
public:
	ScrollBar() { }
	ScrollBar(Rectangle bounds, float min_scroll, float font_size, size_t item_count);

	void draw();

	void set_bounds(Rectangle bounds) { this->bounds = bounds; }
	void set_max_scroll(float max_scroll) { this->max_scroll = max_scroll; }
	void set_scroll_val(float scroll_val) { this->scroll_val = scroll_val; }

	float get_max_scroll() { return max_scroll; }
	float get_scroll_val() { return scroll_val; }
private:
	Rectangle bounds = {};

	float min_scroll = 0;
	float max_scroll = 0;
	float scroll_val = 0;
};

class Slider {
public:
	Slider(Rectangle bounds, string text_left, string text_right, float val, float min_val, float max_val) : 
		bounds{ bounds }, text_left{ text_left }, text_right{ text_right }, val{ val }, min_val{ min_val }, max_val{ max_val } 
	{ old_val = val; }
	void draw() { GuiSlider(bounds, text_left.c_str(), text_right.c_str(), &val, min_val, max_val); }
	
	float get_val() { return val; }
	void set_val(float val) { this->val = val; }

	bool is_updated() { return val != old_val; }
	void update()	  { old_val = val; }
private:
	Rectangle bounds;
	string text_left;
	string text_right;

	float val;
	float min_val;
	float max_val;

	float old_val;
};

class Panel {
public:
	Panel(Rectangle bounds, const char *text) : bounds{ bounds }, text{ text } { }
	void draw() { GuiPanel(bounds, text); }
private:
	Rectangle bounds;
	const char *text;
};

class ColorToggle {
public:
	ColorToggle(Rectangle bounds, Color color) : bounds{ bounds }, color{ color } {
		color_bounds = { 
			bounds.x + bounds.width * 0.2f, 
			bounds.y + bounds.height * 0.2f, 
			bounds.width * 0.6f, 
			bounds.height * 0.6f };
	}

	void draw() { 
		if (GuiToggle(bounds, nullptr, &active)) { active = !active; }
		DrawRectangleRec(color_bounds, color);
	}

	bool is_active() { return active; }
	bool is_updated() { return active != active_old; }
	void update() { active_old = active; }

	void set_active(bool active) { this->active = active; }

	Color get_color() { return color; }
private:
	Rectangle bounds;
	Rectangle color_bounds;
	Color color;

	bool active = false;
	bool active_old = false;
};

class ColorGroup {
public:
	ColorGroup(Rectangle toggle_bounds, float offset_x, float offset_y, vector<Color> colors, int toggles_per_row = 1, int active = 0);

	void draw() { for (ColorToggle& ct : toggles) { ct.draw(); } }
	void update();

	bool is_updated();

	Color get_active_color() { return toggles[active].get_color(); }
	int get_active() { return active; }
private:
	vector<ColorToggle> toggles;
	int active;

	int active_new;
};

class RangeSlider {
public:
	RangeSlider(Rectangle bounds, string text_left, string text_right, float min_val, float max_val);

	void draw();
	bool is_updated() { return updated; }
	void update() { updated = false; }

	void set_text_color(Color color) { range->set_color(color); }

	float get_min() { return min_slider->get_val(); }
	float get_max() { return max_slider->get_val(); }

	~RangeSlider();
private:
	TextEx* range = nullptr;
	Slider* min_slider = nullptr;
	Slider* max_slider = nullptr;

	bool updated = false;
};