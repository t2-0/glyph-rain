#pragma once
#include "raylib_raygui.h"
#include "gui/Gui.h"
#include <vector>

using std::string;
using std::vector;

// Represents a single animated glyph.
//
// Responsibilities:
// - Store and render a character
// - Animate symbol changes over time
// - Handle randomized update intervals
//
// Notes:
// - Uses shared static font for all glyphs
// - Characters are randomized using ASCII uppercase letters
// - Update speed is affected by global_speed multiplier
class Glyph {
public:
	Glyph(char c) : c{ c } { }

	void draw(Vector2 position, Color color) const;
	void update(float global_speed);

	static void set_font(Font font_n) { font = font_n; }
protected:
	static Font font;
	char c[2];
private:
	float timer = 0.0f;
	float interval = 0.5f;
};

// Represents a vertical falling glyph rain column.
//
// Responsibilities:
// - Store and manage a sequence of glyphs
// - Handle movement and screen wrapping
// - Render glyphs with head/tail fading effect
//
// Notes:
// - Column speed and position are randomized
// - head_region controls brightness/fade distribution
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