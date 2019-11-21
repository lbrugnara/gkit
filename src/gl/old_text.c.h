#include <ft2build.h>
#include FT_FREETYPE_H 
#include <glad/glad.h>

#include "text.h"
#include "internal/shader.h"
#include "shader.h"


struct GKitCharacter {
    unsigned int id;  // ID handle of the glyph texture
    unsigned int width;
    unsigned int height;
    unsigned int left;
    unsigned int top;
    unsigned int advance;    // Offset to advance to next glyph
};

// ASCII
struct GKitCharacter characters[128];

static const char *vertex_shader =
    "#version 330 core"                                                 "\n"
    "layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>"     "\n"
    "out vec2 TexCoords;"                                               "\n"
    ""                                                                  "\n"
    "uniform mat4 projection;"                                          "\n"
    ""                                                                  "\n"
    "void main()"                                                       "\n"
    "{"                                                                 "\n"
    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"         "\n"
    "    TexCoords = vertex.zw;"                                        "\n"
    "}"                                                                 "\n"
;

static const char *fragment_shader =
    "#version 330 core"                                                     "\n"
    "in vec2 TexCoords;"                                                    "\n"
    "out vec4 color;"                                                       "\n"
    ""                                                                      "\n"
    "uniform sampler2D text;"                                               "\n"
    "uniform vec3 textColor;"                                               "\n"
    ""                                                                      "\n"
    "void main()"                                                           "\n"
    "{"                                                                     "\n"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"   "\n"
    "    color = vec4(textColor, 1.0) * sampled;"                           "\n"
    "}"                                                                     "\n"
;


// Temporal
GLuint VAO, VBO;

static void init_characters_map(FT_Face face)
{
    FT_Set_Pixel_Sizes(face, 0, 48);

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
        characters[(int)character] = (struct GKitCharacter){
            .id = texture_id,
            .width = face->glyph->bitmap.width,
            .height = face->glyph->bitmap.rows,
            .left = face->glyph->bitmap_left,
            .top = face->glyph->bitmap_top,
            .advance = face->glyph->advance.x,
        };
    }

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int gkit_text_init(void)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        fprintf(stderr, "Could not initialize FreeType");
        return 0;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face))
    {
        fprintf(stderr, "Failed to load font");
        return 0;
    }
    
    init_characters_map(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
        
    return 1;
}

void gkit_text_render(const char *text, GLfloat x, GLfloat y)
{
    if (characters[0].id == 0)
        gkit_text_init();
    
    struct GKitShader *shader = gkit_shader_create(vertex_shader, fragment_shader);
    
    GLfloat scale = 1.0f;
    // Activate corresponding render state	
    glUseProgram(*(unsigned int*)shader->raw);
    glUniform3f(glGetUniformLocation(*(unsigned int*)shader->raw, "textColor"), 0.5, 0.8f, 0.2f);

    float left = 0.0f;
    float right = 800.0f;
    float bottom = 0.0f;
    float top = 600.0f;
    float nearVal = 0.0f;
    float farVal = 1.0f;
    const float ortho[] = {
        2.0f / (right - left),                  0.0f,                           0.0f,   -1.0f * ((right + left) / (right - left)),
        0.0f,                   2.0f / (top - bottom),                          0.0f,   -1.0f * ((top + bottom) / (top - bottom)),
        0.0f,                                   0.0f,     -2.0f / (farVal - nearVal),   -1.0f * ((farVal + nearVal) / (farVal - nearVal)),
        0.0f,                                   0.0f,                           0.0f,    1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(*(unsigned int*)shader->raw, "projection"), 1, GL_FALSE, ortho);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    for (char *charptr = (char*)text; *charptr; charptr++) 
    {
        struct GKitCharacter ch = characters[(int)*charptr];

        GLfloat xpos = x + ch.left * scale;
        GLfloat ypos = y - (ch.height - ch.top) * scale;

        GLfloat w = ch.width * scale;
        GLfloat h = ch.height * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.id);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    gkit_shader_destroy(shader);
}
