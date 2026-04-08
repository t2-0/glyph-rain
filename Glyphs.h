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

// implemented for string/image input
class GlyphStatic : public Glyph {
public:
	GlyphStatic(char c, Vector2 position)
		       : Glyph{ c }, position{ position } { }

	void draw(Color color) { DrawTextEx(font, c, position, font.baseSize, 0.0f, color); }
	void set_active(bool active) { this->active = active; }

	Vector2 get_position() const { return position; }
	bool get_active() { return active; }
private:
	using Glyph::update;
	using Glyph::get_char;

	Vector2 position;
	bool active = false;
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


class GlyphRender {
public:
	//void draw();
	void clear() { UnloadRenderTexture(texture); points.clear(); }
	virtual void extract(string text) = 0;

	vector<pair<Vector2, int>>& get_points() { return points; }
	int get_left()  { return left_x; }
	int get_right() { return right_x; }

	void set_position(Vector2 position) { this->position = position; }
protected:
	RenderTexture2D texture = {};
	vector<pair<Vector2, int>> points;
	Vector2 position = { 0.0f, 0.0f };

	int left_x = GetScreenWidth();
	int right_x = 0;

	void load_points(Image image, Vector2 position, int step);
};

class GlyphText : public GlyphRender {
public:
	GlyphText(Font font, int font_size) : font{ font }, font_size{ font_size } {}

	void extract(string text) override;
private:
	Font font;
	int font_size;
};

class GlyphImage : public GlyphRender {
public:
	GlyphImage(Vector2 size) : size{ size } { }

	void extract(string file_name) override;
private:
	Vector2 size;
};

class GlyphInput {
public:
	GlyphInput(Font font, int font_size) { texture = make_unique<GlyphText>(font, font_size); }
	GlyphInput(Vector2 size) { texture = make_unique<GlyphImage>(size); }

	void draw(const Gui& gui);
	void update(GlyphColumn& column);
	void update_texture(string text);

	void clear_texture() { glyphs.clear(); texture->clear(); }
	void set_position(Vector2 position) { texture->set_position(position); };

	~GlyphInput() {}
private:
	unique_ptr<GlyphRender> texture;
	vector<GlyphStatic> glyphs;
};
