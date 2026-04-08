#include "utils.h"
#include <string>
#include <vector>
#include <numeric>

using std::string;
using std::vector;
using std::iota;

unique_ptr<Font> merge_noto_fonts(int font_size) {
    // load files
    int size_sans = 0;
    int size_emoji = 0;

    unsigned char* data_sans  = LoadFileData("NotoSans-Regular.ttf",  &size_sans);
    unsigned char* data_emoji = LoadFileData("NotoEmoji-Regular.ttf", &size_emoji);

    // ranges
    vector<int> text_range (0x052F - 0x0020 + 1);
    iota(text_range.begin(), text_range.end(), 0x0020);
    
    vector<int>  emoji_range (0x1F6FF - 0x2600 + 1);
    iota(emoji_range.begin(), emoji_range.end(), 0x2600);

    GlyphInfo* glyphs_text = LoadFontData(data_sans, size_sans, font_size,
                                         text_range.data(), text_range.size(), FONT_DEFAULT);
    GlyphInfo* glyphs_emoji = LoadFontData(data_emoji, size_emoji, font_size, 
                                         emoji_range.data(), emoji_range.size(), FONT_DEFAULT);

    int text_count  = text_range.size();
    int emoji_count = emoji_range.size();
    int total_count = text_count + emoji_count;

    // combine sans and emoji
    vector<GlyphInfo> all_glyphs(total_count);
    for (int i = 0; i < text_count; i++) {
        all_glyphs[i] = glyphs_text[i];
        all_glyphs[i].image = ImageCopy(glyphs_text[i].image);
    }

    for (int i = 0; i < emoji_count; i++) {
        all_glyphs[text_count + i] = glyphs_emoji[i];
        all_glyphs[text_count + i].image = ImageCopy(glyphs_emoji[i].image);
    }

    Rectangle* recs = nullptr;
    Image atlas = GenImageFontAtlas(all_glyphs.data(), &recs, total_count, font_size, 4, 0);

    // merge
    unique_ptr<Font> merged_font = make_unique<Font>(GuiGetFont());
    merged_font->baseSize = font_size;
    merged_font->glyphCount = total_count;

    merged_font->glyphs = (GlyphInfo*)MemAlloc(sizeof(GlyphInfo) * total_count);
    for (int i = 0; i < total_count; i++) {
        merged_font->glyphs[i] = all_glyphs[i];
        merged_font->glyphs[i].image = ImageCopy(all_glyphs[i].image); 
    }

    merged_font->recs = recs;
    merged_font->texture = LoadTextureFromImage(atlas);

    UnloadFontData(glyphs_text, text_range.size());
    UnloadFontData(glyphs_emoji, emoji_range.size());
    UnloadImage(atlas);

    UnloadFileData(data_sans);
    UnloadFileData(data_emoji);
    for (auto& g : all_glyphs) { UnloadImage(g.image); }

    return merged_font;
}