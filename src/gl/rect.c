#include <stdio.h>
#include <glad/glad.h>
#include <fllib.h>
#include "rect.h"
#include "calc.h"
#include "../shader.h"
#include "../internal/shader.h"
#include "../internal/element.h"

static inline bool gkit_gl_rect_is_initialized(struct GLElementRect *glElement)
{
    return glElement->sid != 0
            && glElement->vao != 0
            && glElement->vbo != 0
            && glElement->ebo != 0;
}

bool gkit_gl_rect_initialize(struct GLElementRect *glElement)
{
    if (!glElement)
        return false;

    glElement->shader = gkit_shader_create_default();

    if (!glElement->shader)
        return false;

    glElement->sid = *(unsigned int*)glElement->shader->raw;

    glGenVertexArrays(1, &glElement->vao);
    glBindVertexArray(glElement->vao);

    glGenBuffers(1, &glElement->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glElement->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glElement->vertices), glElement->vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &glElement->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glElement->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glElement->indices), glElement->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return true;
}

bool gkit_gl_rect_create(struct GKitElement *element)
{
    if (element == NULL || element->type != GKIT_ELEMENT_RECT || element->raw != NULL)
        return false;

    element->raw = fl_malloc(sizeof(struct GLElementRect));
    struct GLElementRect *glElement = (struct GLElementRect*)element->raw;
    memcpy(glElement->indices, (int[6]){ 0, 1, 3, 1, 2, 3 }, sizeof(glElement->indices));

    return true;
}

bool gkit_gl_rect_draw(struct GKitElement *element, struct GKitViewport viewport)
{
    struct GLElementRect *glElement = (struct GLElementRect*)element->raw;

    if (!gkit_gl_rect_is_initialized(glElement) && !gkit_gl_rect_initialize(glElement))
        return false;

    glUseProgram(glElement->sid);
    glBindVertexArray(glElement->vao);

    float top = gkit_calc_element_top_ndc(&viewport, element);
    float left = gkit_calc_element_left_ndc(&viewport, element);
    float right = gkit_calc_element_right_ndc(&viewport, element);
    float bottom = gkit_calc_element_bottom_ndc(&viewport, element);
    float zIndex = gkit_calc_element_z_index_ndc(&viewport, element);

    glElement->vertices[0] = left;
    glElement->vertices[1] = top;
    glElement->vertices[2] = zIndex;

    glElement->vertices[3] = right;
    glElement->vertices[4] = top;
    glElement->vertices[5] = zIndex;

    glElement->vertices[6] = right;
    glElement->vertices[7] = bottom;
    glElement->vertices[8] = zIndex;

    glElement->vertices[9] = left;
    glElement->vertices[10] = bottom;
    glElement->vertices[11] = zIndex;

    glBindBuffer(GL_ARRAY_BUFFER, glElement->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glElement->vertices), glElement->vertices, GL_DYNAMIC_DRAW);

    int colorLocation = glGetUniformLocation(glElement->sid, "color");
    glUniform4f(
        colorLocation, 
        (float)element->style.color.red / 255.0f, 
        (float)element->style.color.green / 255.0f, 
        (float)element->style.color.blue / 255.0f, 
        element->style.color.alpha
    );

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return true;
}

void gkit_gl_rect_destroy(struct GKitElement *element)
{
    struct GLElementRect *glElement = (struct GLElementRect*)element->raw;
    glDeleteVertexArrays(1, &glElement->vao);
    glDeleteBuffers(1, &glElement->vbo);
    glDeleteBuffers(1, &glElement->ebo);
    gkit_shader_destroy(glElement->shader);
    fl_free(glElement);
}
