#include "image_input/ImageInput.h"
#include "utils/utils.h"

int ASCIIRenderer::step = 0;

void ASCIIRenderer::update_cells(const Image& image) {
	cells.clear();
	Color* pixels = LoadImageColors(image);
	if (!pixels) { return; }

	for (int y = 0; y < rows; y += step) {
		for (int x = 0; x < cols; x += step) {
			int img_x = x * (image.width - 1) / (cols - 1);
			int img_y = y * (image.height - 1) / (rows - 1);

			Color color = pixels[img_y * image.width + img_x];
			float brightness = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;

			string ramp = ".:-=+*#%@";
			int idx = (brightness / 255.0f) * (ramp.size() - 1);

			Vector2 cell_pos = position;
			cell_pos.x += x;
			cell_pos.y += y;

			cells.push_back({ ramp[idx], cell_pos, color });
		}
	}

	UnloadImageColors(pixels);
}

void ASCIIRenderer::draw(bool keep_colors, float head_region) {
	if (hide) return;
	float tail_region = 1.0f - head_region;

	for (int i = 0; i < cells.size(); i++) {
		Color color;
		if (keep_colors) {
			color = adjust_color(cells[i].color, i, cells.size(), tail_region, head_region);
		}
		else {
			Color text_color = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
			color = adjust_color(text_color, i, cells.size(), tail_region, head_region);
		}

		DrawTextEx(font, cells[i].c, cells[i].pos, font.baseSize, 0.0f, color);
	}
}

void ASCIIPreview::draw(bool keep_colors, float head_region) {
	ASCIIRenderer::draw(keep_colors, head_region);

	Color line_color = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL));
	DrawRectangleLinesEx(bounds, 1.0f, line_color);
}

void ImageInput::draw(bool keep_colors, float head_region) {
	renderer.draw(keep_colors, head_region);
}