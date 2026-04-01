#pragma once
#include "raylib_raygui.h"
#include "GUI.h"
#include <vector>

using std::vector;

class Glyph {
public:
	Glyph(char c) : c{ c } { }

	void draw(Vector2 position, Color color) const;
	void update(float global_speed);

	static void set_rotation(float rotation_n) { rotation = rotation_n; }
	static void set_font(Font font_n) { font = font_n; }
private:
	static float rotation;
	static Font font;

	char c[2];
	float timer = 0.0f;
	float interval = 0.5f;
};

class GlyphColumn {
public:
	GlyphColumn(int size, Vector2 position, Font font);

	void draw();
	void update(const GUI& gui);
	// (position.y + (column.size() * y_offset)) == head pos
	bool out_of_height() { return (position.y + (column.size() * y_offset)) <= 0.0f; }
private:
	vector<Glyph> column;
	Vector2 position;

	float y_offset = 0.0f;
	float speed = 0.0f;
	int target_size;

	static float head_region;
};