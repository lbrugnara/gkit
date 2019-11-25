#include <float.h>
#include <glad/glad.h>
#include "text.h"
#include "calc.h"
#include "../shader.h"
#include "../internal/font.h"
#include "../internal/element.h"

struct GKitFont* default_font;

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

    const char *vertex_shader = fl_io_file_read_all_text("resources/shaders/text.vs.glsl");
    const char *fragment_shader = fl_io_file_read_all_text("resources/shaders/text.fs.glsl");

    glElement->shader = gkit_shader_create(vertex_shader, fragment_shader);

    fl_cstring_free(fragment_shader);
    fl_cstring_free(vertex_shader);

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

    // Prepare
    glUseProgram(glElement->sid);

    glUniform4f(
        glGetUniformLocation(glElement->sid, "textColor"),
        (float)element->style.color.red / 255.0f, 
        (float)element->style.color.green / 255.0f, 
        (float)element->style.color.blue / 255.0f, 
        element->style.color.alpha
    );


    // TODO: An overflow property should set this accordingly to let the fragment shader
    // discard fragments outside the rectangle bounds
    int box_overflow_left_px =   /*0;                 //*/  gkit_calc_element_left_px(&viewport, element);
    int box_overflow_right_px =  /*viewport.width;    //*/  gkit_calc_element_right_px(&viewport, element);
    int box_overflow_top_px =    /*viewport.height;   //*/  viewport.height - gkit_calc_element_top_px(&viewport, element);
    int box_overflow_bottom_px = /*0;                 //*/  viewport.height- gkit_calc_element_bottom_px(&viewport, element);

    glUniform1i(glGetUniformLocation(glElement->sid, "left"), box_overflow_left_px);
    glUniform1i(glGetUniformLocation(glElement->sid, "right"), box_overflow_right_px);
    glUniform1i(glGetUniformLocation(glElement->sid, "top"), box_overflow_top_px);
    glUniform1i(glGetUniformLocation(glElement->sid, "bottom"), box_overflow_bottom_px);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glElement->vao);

    // Render    

    // The 'x' value is our "pen" while we are writing, we start at the rect's left-most
    // side and we 'advance' to the next 'x' after writing each character
    float character_x_px = gkit_calc_element_left_px(&viewport, element);

    // We start to write at the top-left side within the rect, so our starting
    // point is the rect's top value (plus a calculation based on each character)
    int box_top_px = gkit_calc_element_top_px(&viewport, element);

    // font-size
    float scale = 1.0f;

    for (char *charptr = textElement->content; charptr && *charptr; charptr++) 
    {
        struct GKitCharacter ch = default_font->alphabet[(int)*charptr];

        // We scale all the values based on the font-size
        float ch_left = ch.left * scale;
        float ch_width = ch.width * scale;
        float ch_height = ch.height * scale;
        float ch_top = ch.top * scale;
        float gt_top = default_font->gt_top * scale;

        // The left vertex is the current "pen" position + the character's bearingX (left)
        float left = gkit_calc_x_value_ndc(&viewport, character_x_px + ch_left);

        // The right vertex is the current "pen" position + the character's bearingX (left) + the character width
        float right = gkit_calc_x_value_ndc(&viewport, character_x_px + ch_left + ch_width);

        // The bottom vertex is the rect's top value + the greatest top (bearingY) in the font + the difference
        // between the character height - the character top:
        //      'a', 'b', and 'c' are placed "on" the imaginary x-axis because they have the same value for top 
        //      and height (the difference is 0). The same applies for all the other characters with diff==0.
        //      On the other hand, characters like 'g' or 'y' have a diff>0 and the bottom line is placed "below"
        //      the x-axis.
        float bottom = gkit_calc_y_value_ndc(&viewport, box_top_px + gt_top + (ch_height - ch_top));

        // The top position is easier to calculate, it is the rect's top value + the difference between the greatest
        // top in the alphabet and the current character's top
        float top    = gkit_calc_y_value_ndc(&viewport, box_top_px + (gt_top - ch_top));

        float zIndex = gkit_calc_element_z_index_ndc(&viewport, element);


        GLfloat vertices[6][3] = {
            // X           Y         Z
            { left,  top,       zIndex },
            { left,  bottom,    zIndex },
            { right, bottom,    zIndex },

            { left,  top,       zIndex },
            { right, bottom,    zIndex },
            { right, top,       zIndex },
        };

        // Texture mapping
        GLfloat texture[6][2] = {
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
        // Send the vertex data
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // Send the texture data
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texture), texture);

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
