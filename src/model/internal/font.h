#ifndef GKIT_INTERNAL_FONT_H
#define GKIT_INTERNAL_FONT_H

#include <stdbool.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include "../font.h"

struct GKitCharacter {
    unsigned int id;
    unsigned int width;
    unsigned int height;
    int left;
    int top;
    unsigned int advance;
};

struct GKitFont {
    // FIXME: BY now we only support ASCII
    struct GKitCharacter alphabet[128];
    unsigned int gt_height;
    int gt_top;
};

bool gkit_internal_font_load_alphabet(GKitFont font, FT_Face face);

#endif /* GKIT_INTERNAL_FONT_H */
