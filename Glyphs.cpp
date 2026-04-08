#include "Glyphs.h"
#include <stdlib.h>  
#include <iostream>

Font Glyph::font;

float GlyphColumn::head_region = 0.2f;

using namespace std;

Color adjust_color(int i, int size, float tail_region, float head_region) {
	Color column_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	Color glyph_color;

	float t = i / (size - 1.0f);
	float min_alpha = 65.0f; 
	float max_alpha = 255.0f;

	if (t > tail_region) {
		float factor = (t - tail_region) / head_region;
		factor = factor * factor * (3.0f - 2.0f * factor);

		glyph_color.r = column_color.r + (255.0f - column_color.r) * factor;
		glyph_color.g = column_color.g + (255.0f - column_color.g) * factor;
		glyph_color.b = column_color.b + (255.0f - column_color.b) * factor;
		glyph_color.a = 255.0f;
	}
	else {
		glyph_color = column_color;
		float fade = sqrtf(t / tail_region);

		glyph_color.a = min_alpha + (max_alpha - min_alpha) * fade;
	}

	return glyph_color;
}

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
	Color column_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));

	Vector2 glyph_position = position;
	float tail_region = 1.0f - head_region;

	for (int i = 0; i < column.size(); i++) {
		Color glyph_color = adjust_color(i, column.size(), tail_region, head_region);

		column[i].draw(glyph_position, glyph_color);
		glyph_position.y += y_offset;
	}
}

void GlyphColumn::update(const Gui& gui) {
	if (gui.is_size_updated()) {
		int max_size = gui.get_max_col_size();
		int min_size = gui.get_min_col_size();

		int size = GetRandomValue(min_size, max_size);
		target_size = size;
	}

	float dt = GetFrameTime();
	position.y += speed * dt * gui.get_column_speed();
	head_region = gui.get_head_region();

	if (position.y > GetScreenHeight()) {
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

// debug
/*
void GlyphRender::draw() {
	for (auto [v, i] : points) {
		DrawCircle(v.x, v.y, 1.0f, PURPLE);
	}
}
*/

void GlyphRender::load_points(Image image, Vector2 position, int step) {
    points.clear();

    Color* pixels = LoadImageColors(image);
	int i = 0;
	for (int y = 0; y < image.height; y += step) {
        for (int x = 0; x < image.width; x += step) {
            Color c = pixels[y * image.width + x];
            float brightness = 0.299f * c.r + 0.587f * c.g + 0.114f * c.b;

            if (brightness > 128.0f) {
                float pos_x = position.x + x;

                if (pos_x < left_x) { left_x = pos_x; }
                else if (pos_x > right_x) { right_x = pos_x; }

				points.push_back({ { position.x + x, position.y + y }, i });
				i++;
            }
        }
    }

    UnloadImageColors(pixels);
}

void GlyphText::extract(string text) {
    UnloadRenderTexture(texture);

    Image image = { 0 };
    Vector2 size = MeasureTextEx(font, text.c_str(), font_size, 1.0f);

    texture = LoadRenderTexture(size.x, size.y);
    Rectangle source = { 0, 0, (float)texture.texture.width, -(float)texture.texture.height };

    BeginTextureMode(texture);
    ClearBackground(BLACK);
    DrawTextEx(font, text.c_str(), { 0, 0 }, font_size, 1.0f, WHITE);
    EndTextureMode();

    image = LoadImageFromTexture(texture.texture);
    ImageFlipVertical(&image);

    int step = 3;
    load_points(image, position, step);
}
void GlyphImage::extract(string file_name) {
    Image image = LoadImage(file_name.c_str());
    ImageResize(&image, size.x, size.y);

    int step = 8;
    load_points(image, position, step);
}

void GlyphInput::draw(const Gui& gui) {
   // texture->draw();
	float head_region = gui.get_head_region();
	float tail_region = 1.0f - head_region;
	for (int i = 0; i < glyphs.size(); i++) {
		Color glyph_color = adjust_color(i, glyphs.size(), tail_region, head_region);

		if (glyphs[i].get_active()) {
			glyphs[i].draw(glyph_color);
		}
	}
}

void GlyphInput::update(GlyphColumn& column) {
    Vector2 column_pos = column.get_head_pos();
    vector<pair<Vector2, int>>& points = texture->get_points();

    if (!points.empty() && !column.empty()) {
        float front_y = points.begin()->first.y;
        float back_y  = prev(points.end())->first.y;

        float left_x  = texture->get_left();
        float right_x = texture->get_right();

        bool in_bounds_x = column_pos.x >= (left_x  - 5.0f)
						&& column_pos.x <= (right_x + 5.0f);

        bool in_bounds_y = column_pos.y >= (front_y - 5.0f)
						&& column_pos.y <= (back_y  + 5.0f);

        if (in_bounds_x && in_bounds_y) {
            auto lower = lower_bound(points.begin(), points.end(), column_pos.y - 5.0f,
                [](const pair<Vector2, int>& p, float y) { return p.first.y < y; });

            auto upper = upper_bound(points.begin(), points.end(), column_pos.y + 5.0f,
                [](float y, const pair<Vector2, int>& p) { return y < p.first.y; });

            for (auto it = lower; it != upper; it++) {
                bool near = fabs(column_pos.x - it->first.x) < 5.0f;

                if (near) {
					glyphs[it->second].set_active(true);

                    column.pass();
                    points.erase(it);
                    break;
                }
            }
        }
    }
}

void GlyphInput::update_texture(string text) {
    texture->extract(text);
    glyphs.clear();

	vector<pair<Vector2, int>> points = texture->get_points();
	for (int i = 0; i < points.size(); i++) {
		glyphs.push_back({ 'o', points[i].first});
	}

	sort(glyphs.begin(), glyphs.end(),
		[](const GlyphStatic& g1, const GlyphStatic& g2) {
			return g1.get_position().y < g2.get_position().y;});
}