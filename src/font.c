#include <ft2build.h>
#include FT_FREETYPE_H 

#include "internal/font.h"
#include "font.h"
#include "core.h"

struct GKitFont* default_font = NULL;

struct GKitFont* gkit_font_load(const char *file)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        fprintf(stderr, "Could not initialize FreeType");
        return NULL;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, file, 0, &face))
    {
        fprintf(stderr, "Failed to load font");
        return NULL;
    }

    struct GKitFont* font = fl_malloc(sizeof(struct GKitFont));
    font->gt_height = 0;
    font->gt_top = 0;

    default_font = font;
    
    bool result = gkit_internal_font_load_alphabet(font, face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    if (!result)
    {
        fl_free(font);
        return NULL;
    }

    return font;
}

void gkit_font_free(struct GKitFont *font)
{
    fl_free(font);
}
