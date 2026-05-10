#pragma once
#include <vector>
#include "raylib_raygui.h"
#include "cell/Cell.h"

// Converts image data into ASCII-style rendered cells.
//
// Responsibilities:
// - Sample image pixels
// - Convert brightness into ASCII characters
// - Store generated ASCII cells
// - Render ASCII image representation
//
// Notes:
// - Rendering resolution is controlled by step
// - Supports colored and monochrome rendering modes
// - Uses brightness ramp for ASCII character selection
// - hide flag disables rendering without clearing data
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

// Specialized ASCII renderer with preview bounds visualization.
//
// Responsibilities:
// - Render ASCII image preview
// - Draw preview boundary/frame
//
// Notes:
// - Inherits all ASCII generation/rendering behavior
// - Preview is always visible by default
// - Adds visual border around render region
class ASCIIPreview : public ASCIIRenderer {
public:
	ASCIIPreview(Rectangle bounds, Vector2 pos, int cols, int rows)
		: bounds{ bounds }, ASCIIRenderer{ pos, cols, rows } { set_hide(false); }
	void draw(bool keep_colors, float head_region);
private:
	Rectangle bounds;
};

// High-level wrapper for ASCII image rendering.
//
// Responsibilities:
// - Load images into ASCII representation
// - Control renderer visibility
//
// Notes:
// - Internally uses ASCIIRenderer
// - Acts as simplified interface for image-based ASCII rendering
// - Image data itself is not stored after conversion
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