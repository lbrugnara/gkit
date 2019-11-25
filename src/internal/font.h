#ifndef GKIT_FONT_INTERNAL_H
#define GKIT_FONT_INTERNAL_H

#include <fllib.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

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

bool gkit_internal_font_load_alphabet(struct GKitFont *font, FT_Face face);

#endif /* GKIT_FONT_INTERNAL_H */
