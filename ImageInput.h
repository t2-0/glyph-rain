#pragma once
#include <vector>
#include "raylib_raygui.h"
#include "Cell.h"
#include<iostream>

using namespace std;

class ASCIIRenderer {
public:
	ASCIIRenderer(Vector2 pos, int cols, int rows) : position{ pos }, cols{ cols }, rows{ rows } {}

	void draw(bool keep_colors, float head_region);
	void clear() { UnloadRenderTexture(texture); cells.clear(); }
	void update_cells(const Image& image);

	void set_hide(bool hide) { this->hide = hide; }

	static void set_step(int step_n) { step = step_n; }
protected:
	RenderTexture2D texture = {};
	Vector2 position = { 0.0f, 0.0f };
	vector<Cell> cells;

	int cols = 0;
	int rows = 0;
	bool hide = true;

	Font font = GetFontDefault();

	static int step;
};

class ASCIIPreview : public ASCIIRenderer {
public:
	ASCIIPreview(Rectangle bounds, Vector2 pos, int cols, int rows)
		: bounds{ bounds }, ASCIIRenderer{ pos, cols, rows } { set_hide(false); }
	void draw(bool keep_colors, float head_region);
private:
	Rectangle bounds;
};

class ImageInput {
public:
	ImageInput(Vector2 pos, int cols, int rows) : renderer{ pos, cols, rows } {}

	void draw(bool keep_colors, float head_region);
	void load(const Image& image) { renderer.update_cells(image); }
	void unload() { renderer.clear(); }

	void hide() { renderer.set_hide(true); }
	void show() { renderer.set_hide(false); }
private:
	ASCIIRenderer renderer;
};