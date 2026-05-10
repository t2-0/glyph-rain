#include "utils.h"

Color adjust_color(Color color, int i, int size, float tail_region, float head_region) {
    Color glyph_color;

    float t = i / (size - 1.0f);
    float min_alpha = 65.0f;
    float max_alpha = 255.0f;

    if (t > tail_region) {
        float factor = (t - tail_region) / head_region;
        factor = factor * factor * (3.0f - 2.0f * factor);

        glyph_color.r = color.r + (255.0f - color.r) * factor;
        glyph_color.g = color.g + (255.0f - color.g) * factor;
        glyph_color.b = color.b + (255.0f - color.b) * factor;
        glyph_color.a = 255.0f;
    }
    else {
        glyph_color = color;
        float fade = sqrtf(t / tail_region);

        glyph_color.a = min_alpha + (max_alpha - min_alpha) * fade;
    }

    return glyph_color;
}