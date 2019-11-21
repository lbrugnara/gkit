#include <glad/glad.h>
#include "text.h"
#include "viewport.h"
#include "../shader.h"
#include "../internal/font.h"
#include "../internal/element.h"

struct GKitFont* default_font;

static const char *vertex_shader =
    "#version 330 core"                                                 "\n"
    "layout (location = 0) in vec3 vertex;"                             "\n"
    "layout (location = 1) in vec2 texture;"                            "\n"
    "out vec2 TexCoords;"                                               "\n"
    ""                                                                  "\n"
    "void main()"                                                       "\n"
    "{"                                                                 "\n"
    "    gl_Position = vec4(vertex.xyz, 1.0);"                          "\n"
    "    TexCoords = texture;"                                          "\n"
    "}"                                                                 "\n"
;

static const char *fragment_shader =
    "#version 330 core"                                                     "\n"
    "in vec2 TexCoords;"                                                    "\n"
    "out vec4 color;"                                                       "\n"
    ""                                                                      "\n"
    "uniform sampler2D text;"                                               "\n"
    "uniform vec4 textColor;"                                               "\n"
    ""                                                                      "\n"
    "void main()"                                                           "\n"
    "{"                                                                     "\n"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"   "\n"
    "    color = textColor * sampled;"                                      "\n"
    "}"                                                                     "\n"
;

static inline float* ortho(struct GKitViewport *viewport)
{
    return (float[])
    {
        2.0f / (viewport->width - 0),           0.0f,                           0.0f,                   -1.0f * ((viewport->width + 0) / (viewport->width - 0)),
        0.0f,                                   2.0f / (viewport->height - 0),  0.0f,                   -1.0f * ((viewport->height + 0) / (viewport->height - 0)),
        0.0f,                                   0.0f,                           -2.0f / (1.0 - 0.0),    -1.0f * (1.0 + 0.0) / (1.0 - 0.0),
        0.0f,                                   0.0f,                           0.0f,                   1.0f
    };
}

static inline bool gkit_gl_text_is_initialized(struct GLElementText *glElement)
{
    return glElement->sid != 0
            && glElement->vao != 0
            && glElement->vbo != 0;
}

static inline bool gkit_gl_text_initialize(struct GLElementText *glElement)
{
    if (!glElement)
        return false;

    glElement->shader = gkit_shader_create(vertex_shader, fragment_shader);

    if (!glElement->shader)
        return false;

    glElement->sid = *(unsigned int*)glElement->shader->raw;

    glGenVertexArrays(1, &glElement->vao);
    glBindVertexArray(glElement->vao);

    glGenBuffers(1, &glElement->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glElement->vbo);
    size_t vertex_size = sizeof(float) * 6 * 3;
    size_t texture_size = sizeof(float) * 6 * 2;
    glBufferData(GL_ARRAY_BUFFER, vertex_size + texture_size, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)vertex_size);

    return true;
}

bool gkit_gl_text_create(struct GKitElement *element)
{
    if (element == NULL || element->type != GKIT_ELEMENT_TEXT || element->raw != NULL)
        return false;

    element->raw = fl_malloc(sizeof(struct GLElementText));
    struct GLElementText *glElement = (struct GLElementText*)element->raw;

    return true;
}

bool gkit_gl_text_draw(struct GKitElement *element, struct GKitViewport viewport)
{
    struct GKitElementText *textElement = (struct GKitElementText*)element;
    struct GLElementText *glElement = (struct GLElementText*)element->raw;

    if (!gkit_gl_text_is_initialized(glElement) && !gkit_gl_text_initialize(glElement))
        return false;

    float scale = 1.0f;

    // Prepare
    glUseProgram(glElement->sid);
    int colorLocation = glGetUniformLocation(glElement->sid, "textColor");
    glUniform4f(
        colorLocation, 
        (float)element->style.color.red / 255.0f, 
        (float)element->style.color.green / 255.0f, 
        (float)element->style.color.blue / 255.0f, 
        element->style.color.alpha
    );
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glElement->vao);

    // Render
    int box_bottom_px = viewport_calc_element_bottom_px(&viewport, element);
    float character_x_px = viewport_calc_element_left_px(&viewport, element);

    for (char *charptr = textElement->content; charptr && *charptr; charptr++) 
    {
        struct GKitCharacter ch = default_font->alphabet[(int)*charptr];

        // Left starts at element's left and we add
        float left = viewport_calc_x_value_ndc(&viewport, character_x_px + ch.left);
        float right = viewport_calc_x_value_ndc(&viewport, character_x_px + ch.left + ch.width);

        float bottom = viewport_calc_y_value_ndc(&viewport, box_bottom_px - (ch.height - ch.top));
        float top    = viewport_calc_y_value_ndc(&viewport, box_bottom_px + ch.top);

        float zIndex = viewport_calc_element_z_index_ndc(&viewport, element);

        GLfloat vertices[6][3] = {
            // X           Y         Z
            { left,  top,       zIndex },
            { left,  bottom,    zIndex },
            { right, bottom,    zIndex },

            { left,  top,       zIndex },
            { right, bottom,    zIndex },
            { right, top,       zIndex },
        };

        GLfloat textures[6][2] = {
            //  TX   TY
            {  0.0, 0.0 },
            {  0.0, 1.0 },
            {  1.0, 1.0 },
            {  0.0, 0.0 },
            {  1.0, 1.0 },
            {  1.0, 0.0 },
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.id);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, glElement->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textures), textures); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        character_x_px += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    // Cleanup
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void gkit_gl_text_destroy(struct GKitElement *element)
{
    struct GLElementText *glElement = (struct GLElementText*)element->raw;
    glDeleteVertexArrays(1, &glElement->vao);
    glDeleteBuffers(1, &glElement->vbo);
    gkit_shader_destroy(glElement->shader);
    fl_free(glElement);
}
