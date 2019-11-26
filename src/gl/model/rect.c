#include <stdio.h>
#include <glad/glad.h>
#include <fllib.h>
#include "calc.h"
#include "rect.h"
#include "../../model/internal/element.h"
#include "../../model/internal/rect.h"
#include "../../model/internal/shader.h"

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

    glElement->shader = gkit_shader_create_from_file("resources/shaders/rect.vs.glsl", "resources/shaders/rect.fs.glsl");

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

bool gkit_internal_rect_create(struct GKitElementRect *element)
{
    if (element == NULL || element->base.type != GKIT_ELEMENT_RECT || element->base.raw != NULL)
        return false;

    element->base.raw = fl_malloc(sizeof(struct GLElementRect));
    struct GLElementRect *glElement = (struct GLElementRect*)element->base.raw;
    memcpy(glElement->indices, (int[6]){ 0, 1, 3, 1, 2, 3 }, sizeof(glElement->indices));

    return true;
}

bool gkit_internal_rect_draw(struct GKitElementRect *element, struct GKitViewport viewport, enum GKitElementType type)
{
    if (type != element->base.type)
        return true;

    struct GLElementRect *glElement = (struct GLElementRect*)element->base.raw;

    if (!gkit_gl_rect_is_initialized(glElement) && !gkit_gl_rect_initialize(glElement))
        return false;

    // Set the program to start processing the drawing properties
    glUseProgram(glElement->sid);

    // Properties for overflow
    glUniform1i(glGetUniformLocation(glElement->sid, "left"), gkit_layout_element_limit_left((GKitElement)element, &viewport));
    glUniform1i(glGetUniformLocation(glElement->sid, "right"), gkit_layout_element_limit_right((GKitElement)element, &viewport));
    glUniform1i(glGetUniformLocation(glElement->sid, "top"), gkit_layout_element_limit_top((GKitElement)element, &viewport));
    glUniform1i(glGetUniformLocation(glElement->sid, "bottom"), gkit_layout_element_limit_bottom((GKitElement)element, &viewport));

    // Start rendering
    glBindVertexArray(glElement->vao);

    float top = gkit_calc_element_top_ndc(&viewport, (GKitElement)element);
    float left = gkit_calc_element_left_ndc(&viewport, (GKitElement)element);
    float right = gkit_calc_element_right_ndc(&viewport, (GKitElement)element);
    float bottom = gkit_calc_element_bottom_ndc(&viewport, (GKitElement)element);
    float zIndex = gkit_calc_element_z_index_ndc(&viewport, (GKitElement)element);

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
        (float)element->base.style.color.red / 255.0f, 
        (float)element->base.style.color.green / 255.0f, 
        (float)element->base.style.color.blue / 255.0f, 
        element->base.style.color.alpha
    );

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return true;
}

void gkit_internal_rect_destroy(struct GKitElementRect *element)
{
    struct GLElementRect *glElement = (struct GLElementRect*)element->base.raw;
    glDeleteVertexArrays(1, &glElement->vao);
    glDeleteBuffers(1, &glElement->vbo);
    glDeleteBuffers(1, &glElement->ebo);
    gkit_shader_destroy(glElement->shader);
    fl_free(glElement);
}
