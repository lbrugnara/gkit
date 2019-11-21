#include <stdio.h>
#include <glad/glad.h>
#include <fllib.h>
#include "element.h"
#include "shader.h"

/*
 * Type: struct GLElement
 *  Keeps track of all the needed resources to draw an element using OpenGL
 * 
 * ===== C =====
 *  struct GLElement {
 *      struct GKitElement element;
 *      unsigned int vao;
 *      unsigned int vbo;
 *      unsigned int ebo;
 *      unsigned int shader;
 *  };
 */
struct GLElement {
    struct GKitElement element;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int shader;
    float vertices[12];
    int indices[6];
};

/*
 * Function: calculate_width_in_pixels
 *  Returns the <GKitElement>'s width in pixels. If the unit of measure is PIXEL, it
 *  returns the value associated to it. In case it is PERCENTAGE, it figures out the width
 *  based on the ancestors and/or the viewport.
 *
 * Parameters:
 *  element - <GKitElement> to calculate its width in pixels.
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s width in pixels
 *
 */
int calculate_width_in_pixels(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.width.unit == GKIT_UNIT_PIXEL)
        return element->style.width.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.width.value.percentage / 100;

    return calculate_width_in_pixels(element->parent, viewport) * element->style.width.value.percentage / 100;
}

/*
 * Function: calculate_height_in_pixels
 *  Returns the <GKitElement>'s height in pixels. If the unit of measure is PIXEL, it
 *  returns the value associated to it. In case it is PERCENTAGE, it figures out the height
 *  based on the ancestors and/or the viewport.
 *
 * Parameters:
 *  element - <GKitElement> to calculate its height in pixels.
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s height in pixels
 *
 */
int calculate_height_in_pixels(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.height.unit == GKIT_UNIT_PIXEL)
        return element->style.height.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.height.value.percentage / 100;

    return calculate_height_in_pixels(element->parent, viewport) * element->style.height.value.percentage / 100;
}

/*
 * Function: calculate_left_in_pixels
 *  Returns the <GKitElement>'s left offset in pixels. If the unit of measure is PIXEL, it
 *  returns the value associated to it along with the ancestors' left offset. In case it is PERCENTAGE,
 *  it figures out the left offset based on the ancestors and/or the viewport.
 *
 * Parameters:
 *  element - <GKitElement> to calculate its left offset in pixels.
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s left offset in pixels
 *
 */
int calculate_left_in_pixels(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.left.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.left.value.pixels;

        return calculate_left_in_pixels(element->parent, viewport) + element->style.left.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.left.value.percentage / 100;

    return calculate_width_in_pixels(element->parent, viewport) * element->style.left.value.percentage / 100;
}

/*
 * Function: calculate_top_in_pixels
 *  Returns the <GKitElement>'s top offset in pixels. If the unit of measure is PIXEL, it
 *  returns the value associated to it along with the ancestors' top offset. In case it is PERCENTAGE,
 *  it figures out the top offset based on the ancestors and/or the viewport.
 *
 * Parameters:
 *  element - <GKitElement> to calculate its top offset in pixels.
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s top offset in pixels
 *
 */
int calculate_top_in_pixels(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.top.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.top.value.pixels;

        return calculate_top_in_pixels(element->parent, viewport) + element->style.top.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.top.value.percentage / 100;

    return calculate_height_in_pixels(element->parent, viewport) * element->style.top.value.percentage / 100;
}

/*
 * Function: calculate_left_in_ndc
 *  Returns the <GKitElement>'s left-most vertex position in NDC (x-axis).
 *
 * Parameters:
 *  element - <GKitElement> to calculate its left position in NDC (x-axis).
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s left position in NDC (x-axis)
 *
 */
float calculate_left_in_ndc(GKitElement element, struct GKitViewport *viewport)
{
    float left = calculate_left_in_pixels(element, viewport);

    return (2.0f *  left) / viewport->width - 1.0f;
}

/*
 * Function: calculate_right_in_ndc
 *  Returns the <GKitElement>'s right-most vertex position in NDC (x-axis).
 *
 * Parameters:
 *  element - <GKitElement> to calculate its right position in NDC (x-axis).
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s right position in NDC (x-axis)
 *
 */
float calculate_right_in_ndc(GKitElement element, struct GKitViewport *viewport)
{
    int right = calculate_left_in_pixels(element, viewport) + calculate_width_in_pixels(element, viewport);

    return (2.0f *  right) / viewport->width - 1.0f;
}

/*
 * Function: calculate_top_in_ndc
 *  Returns the <GKitElement>'s top-most vertex position in NDC (x-axis).
 *
 * Parameters:
 *  element - <GKitElement> to calculate its top position in NDC (x-axis).
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s top position in NDC (x-axis)
 *
 */
float calculate_top_in_ndc(GKitElement element, struct GKitViewport *viewport)
{
    float top = calculate_top_in_pixels(element, viewport);

    return (-2.0f *  top) / viewport->height + 1.0f;
}

/*
 * Function: calculate_bottom_in_ndc
 *  Returns the <GKitElement>'s bottom-most vertex position in NDC (x-axis).
 *
 * Parameters:
 *  element - <GKitElement> to calculate its bottom position in NDC (x-axis).
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s bottom position in NDC (x-axis)
 *
 */
float calculate_bottom_in_ndc(GKitElement element, struct GKitViewport *viewport)
{
    int bottom = calculate_top_in_pixels(element, viewport) + calculate_height_in_pixels(element, viewport);

    return (-2.0f *  bottom) / viewport->height + 1.0f;
}

/*
 * Function: calculate_z_index_in_ndc
 *  Returns the <GKitElement>'s z-index vertex position in NDC (x-axis).
 *
 * Parameters:
 *  element - <GKitElement> to calculate its z-index in NDC (x-axis).
 *  viewport - Viewport size.
 *
 * Returns:
 *  int - <GKitElement>'s z-index in NDC (x-axis)
 *
 */
float calculate_z_index_in_ndc(GKitElement element, struct GKitViewport *viewport)
{
    float zIndexf = ((float)element->style.zIndex - GKIT_Z_INDEX_MIN) / (GKIT_Z_INDEX_MAX - GKIT_Z_INDEX_MIN);

    zIndexf = (zIndexf * (-1.0f - 1.0f)) + 1.0f;

    float pzi = element->parent
                    ? calculate_z_index_in_ndc(element->parent, viewport)
                    : 1.0f; // Farther distance

    // Minimum depth is parent's depth
    return pzi < zIndexf ? pzi : zIndexf;
}

inline bool gl_element_is_initialized(struct GLElement *glElement)
{
    return glElement->shader != 0
            && glElement->vao != 0
            && glElement->vbo != 0
            && glElement->ebo != 0;
}

/*
 * Function: gkit_element_create
 *  Allocates memory for a <GLElement> that contains the reference to a <GKitElement>.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 * GKitElement - Instance to the newly created element
 *
 */
GKitElement gkit_element_create(void)
{
    struct GLElement *glElement = fl_malloc(sizeof(struct GLElement));

    memcpy(glElement->indices, (int[6]){ 0, 1, 3, 1, 2, 3 }, sizeof(glElement->indices));

    return &glElement->element;
}


/*
 * Function: gkit_element_add_child
 *  Adds a <GKitElement> as a child of another one. This function allocates in the
 *  parent <GKitElement> the memory to be able to track children elements.
 *
 * Parameters:
 *  parent - Parent <GKitElement> that will keep track of *child*
 *  child - Child <GKitElement>
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_element_add_child(GKitElement parent, GKitElement child)
{
    if (!parent->children)
        parent->children = fl_list_new();

    fl_list_append(parent->children, child);

    child->parent = parent;
}

/*
 * Function: gkit_element_draw
 *  Draws the <GKitElement> and all its children. If *element* has not been
 *  initialized, this function will create a shader for the element. If the shader element
 *  cannot be created, this function will fail and return *false* to indicate the error.
 *
 * Parameters:
 *  element - <GKitElement> to be drawn.
 * viewport - Current viewport's size
 *
 * Returns:
 *  bool - *true* on success. Otherwise it returns *false*.
 */
bool gkit_element_draw(GKitElement element, struct GKitViewport viewport)
{
    struct GLElement *glElement = (struct GLElement*)element;

    if (!gl_element_is_initialized(glElement))
    {
        glElement->shader = gkit_shader_new_default();

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

        if (!gl_element_is_initialized(glElement))
            return false;
    }

    glUseProgram(glElement->shader);
    glBindVertexArray(glElement->vao);

    float top = calculate_top_in_ndc(element, &viewport);
    float left = calculate_left_in_ndc(element, &viewport);
    float right = calculate_right_in_ndc(element, &viewport);
    float bottom = calculate_bottom_in_ndc(element, &viewport);
    float zIndex = calculate_z_index_in_ndc(element, &viewport);

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

    int colorLocation = glGetUniformLocation(glElement->shader, "color");
    glUniform4f(
        colorLocation, 
        (float)element->style.color.red / 255.0f, 
        (float)element->style.color.green / 255.0f, 
        (float)element->style.color.blue / 255.0f, 
        element->style.color.alpha
    );

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (element->children)
    {
        struct FlListNode *node = fl_list_head(element->children);
        while (node) {
            gkit_element_draw((GKitElement)node->value, viewport);
            node = node->next;
        }
    }

    return true;
}


/*
 * Function: gkit_element_destroy
 *  Frees all the resources allocated for the <GLElement>, including the <GKitElement>. This function also frees
 *  all the resources for the children <GKitElement>s, that way, freeing the root node of a <GKitWindow> will result
 *  in all its <GKitElement> being freed too.
 *
 * Parameters:
 *  element - <GKitElement> (and <GLElement>) to be freed.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_element_destroy(GKitElement element)
{
    struct GLElement *glElement = (struct GLElement*)element;

    if (element->children)
    {
        struct FlListNode *node = fl_list_head(element->children);
        while (node) {
            gkit_element_destroy((GKitElement)node->value);
            node = node->next;
        }
        fl_list_free(element->children);
    }

    glDeleteVertexArrays(1, &glElement->vao);
    glDeleteBuffers(1, &glElement->vbo);
    glDeleteBuffers(1, &glElement->ebo);
    fl_free(glElement);
}
