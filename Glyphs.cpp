#include "Glyphs.h"
#include "utils.h"
#include <stdlib.h>  
#include <iostream>

Font Glyph::font;

float GlyphColumn::head_region = 0.2f;

using namespace std;

void Glyph::draw(Vector2 position, Color color) const {
	DrawTextEx(font, c, position, font.baseSize, 0.0f, color);
}

void Glyph::update(float global_speed) {
	float dt = GetFrameTime();
	timer += dt * global_speed;

	if (timer >= interval) {
		int codepoint = GetRandomValue(65, 90);

		int utf8Size = 0;
		char utf8 = *CodepointToUTF8(codepoint, &utf8Size);

		c[0] = { utf8 };

		timer = 0.0f;
		interval = 0.05f + (GetRandomValue(0, 1000) / 1000.0f) * (0.2f - 0.05f);
	}
}

GlyphColumn::GlyphColumn(int size, Vector2 position, Font font) {
	this->position = position;
	target_size = size;
	speed = GetRandomValue(150, 350);

	for (int i = 0; i < size; i++) {
		int codepoint = GetRandomValue(65, 90);

		int utf8Size = 0;
		char utf8 = *CodepointToUTF8(codepoint, &utf8Size);

		column.push_back(Glyph { utf8 });
	}

	y_offset = MeasureTextEx(font, "A", font.baseSize, 0.0f).y;
}

void GlyphColumn::draw() {
	Vector2 glyph_position = position;
	float tail_region = 1.0f - head_region;

	for (int i = 0; i < column.size(); i++) {
		Color text_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
		Color glyph_color = adjust_color(text_color, i, column.size(), tail_region, head_region);

		column[i].draw(glyph_position, glyph_color);
		glyph_position.y += y_offset;
	}
}

void GlyphColumn::update(const Gui& gui) {
	head_region = gui.get_head_region();
	if (gui.is_size_updated()) {
		int max_size = gui.get_max_col_size();
		int min_size = gui.get_min_col_size();

		int size = GetRandomValue(min_size, max_size);
		target_size = size;
	}

	float dt = GetFrameTime();
	position.y += speed * dt * gui.get_column_speed();

	if (position.y >= GetScreenHeight()) {
		// smooth resizing
		if (column.size() < target_size) {
			for (int i = 0; i < target_size; i++) {
				int codepoint = GetRandomValue(65, 90);

				int utf8Size = 0;
				char utf8 = *CodepointToUTF8(codepoint, &utf8Size);

				column.push_back({ utf8 });
			}
		}
		else if (column.size() > target_size) {
			column.erase(column.begin() + target_size, column.end());
		}

		speed = GetRandomValue(150, 350);

		position.x = GetRandomValue(20, GetScreenWidth() - 20);
		position.y = -(column.size() * y_offset);
	}

	float glyph_speed = gui.get_glyph_speed();
	for (int i = 0; i < column.size(); i++) {
		column[i].update(glyph_speed);
	}
}