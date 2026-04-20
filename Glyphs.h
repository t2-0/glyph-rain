#pragma once
#include "raylib_raygui.h"
#include "Gui.h"
#include <vector>
#include <algorithm>
#include <unordered_set>

using std::string;
using std::vector;
using std::equal_range;
using std::lower_bound;
using std::upper_bound;

class Glyph {
public:
	Glyph(char c) : c{ c } { }

	void draw(Vector2 position, Color color) const;
	void update(float global_speed);
	char get_char() { return c[0]; }

	static void set_font(Font font_n) { font = font_n; }
protected:
	static Font font;
	char c[2];
private:
	float timer = 0.0f;
	float interval = 0.5f;
};

class GlyphColumn {
public:
	GlyphColumn(int size, Vector2 position, Font font);

	void draw();
	void update(const Gui& gui);
	// (position.y + (column.size() * y_offset)) == head pos
	bool out_of_height() { return (position.y + (column.size() * y_offset)) <= 0.0f; }

	Vector2 get_head_pos() { return { position.x, position.y + (column.size() * y_offset) }; }

	void pass()  { column.pop_back(); }
	bool empty() { return column.empty(); }
private:
	vector<Glyph> column;
	Vector2 position;

	float y_offset = 0.0f;
	float speed = 0.0f;

	int target_size;

	static float head_region;
};