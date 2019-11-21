#include <ft2build.h>
#include FT_FREETYPE_H 
#include <glad/glad.h>

#include "../internal/font.h"
#include "../font.h"

bool gkit_internal_font_load_alphabet(struct GKitFont *font, FT_Face face)
{
    FT_Set_Pixel_Sizes(face, 0, 48);

    GLint default_alignment = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &default_alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
  
    for (size_t i=0; i < 128; i++)
    {
        char character = (char)i;
        // Load character glyph 
        if (FT_Load_Char(face, character, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Couldn't load glyph for character %c", character);
            continue;
        }

        // Generate texture
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        struct GKitCharacter *gkit_char = font->alphabet + (int)character;
        gkit_char->id = texture_id;
        gkit_char->width = face->glyph->bitmap.width;
        gkit_char->height = face->glyph->bitmap.rows;
        gkit_char->left = face->glyph->bitmap_left;
        gkit_char->top = face->glyph->bitmap_top;
        gkit_char->advance = face->glyph->advance.x;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    //glPixelStorei(GL_UNPACK_ALIGNMENT, default_alignment); // Disable byte-alignment restriction

    return true;
}
