#include "../internal/element.h"

/*
 * Function: viewport_calc_element_width_px
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
int viewport_calc_element_width_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    if (element->style.width.unit == GKIT_UNIT_PIXEL)
        return element->style.width.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.width.value.percentage / 100;

    return viewport_calc_element_width_px(viewport, element->parent) * element->style.width.value.percentage / 100;
}

/*
 * Function: viewport_calc_element_height_px
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
int viewport_calc_element_height_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    if (element->style.height.unit == GKIT_UNIT_PIXEL)
        return element->style.height.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.height.value.percentage / 100;

    return viewport_calc_element_height_px(viewport, element->parent) * element->style.height.value.percentage / 100;
}

/*
 * Function: viewport_calc_element_left_px
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
int viewport_calc_element_left_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    if (element->style.left.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.left.value.pixels;

        return viewport_calc_element_left_px(viewport, element->parent) + element->style.left.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.left.value.percentage / 100;

    return viewport_calc_element_width_px(viewport, element->parent) * element->style.left.value.percentage / 100;
}


int viewport_calc_element_right_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    //            Viewport's width
    //      .---------------------------.
    //      |                           |
    //                                    
    //      +---------------------------+
    //      |                           |
    //      |      element's width      |
    //      |       .-----------.       |
    //      |       |           |       |
    //      |                           |
    //      |       +-----------+       |
    //      |       |           |       |
    //      |       |           |       |
    //      |       +-----------+       |
    //      |       ^                   |
    //      |       |                   |
    //      | Element's left            |
    //      +---------------------------+
    //
    // viewport->width - (element->left + element->width) == element->right
    //
    int right_offset = viewport_calc_element_left_px(viewport, element) + viewport_calc_element_width_px(viewport, element);

    return viewport->width - right_offset;
}

/*
 * Function: viewport_calc_element_top_px
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
int viewport_calc_element_top_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    if (element->style.top.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.top.value.pixels;

        return viewport_calc_element_top_px(viewport, element->parent) + element->style.top.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.top.value.percentage / 100;

    return viewport_calc_element_height_px(viewport, element->parent) * element->style.top.value.percentage / 100;
}

int viewport_calc_element_bottom_px(struct GKitViewport *viewport, struct GKitElement *element)
{
    // We first get the bottom offset using the top and height:
    //
    //                 .--   +------------------------------------------------------------+
    //                 |     |                                                            |
    //                 |     |             .--  +-----------+    <--- Element's top       |
    //     Viewport's  |     |   Element's |    |           |                             |
    //       height    |     |    height   |    |           |                             |
    //                 |     |             `--  +-----------+                             |
    //                 |     |                                                            |
    //                  `--  +------------------------------------------------------------+
    //
    // viewport->height - (element->top + element->height) == element->bottom
    //
    int bottom_offset = viewport_calc_element_top_px(viewport, element) + viewport_calc_element_height_px(viewport, element);

    return viewport->height - bottom_offset;
}

/*
 * Function: viewport_calc_element_left_ndc
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
float viewport_calc_element_left_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    float left = viewport_calc_element_left_px(viewport, element);

    return (2.0f *  left) / viewport->width - 1.0f;
}

/*
 * Function: viewport_calc_element_right_ndc
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
float viewport_calc_element_right_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int right_offset = viewport_calc_element_left_px(viewport, element) + viewport_calc_element_width_px(viewport, element);

    return (2.0f *  right_offset) / viewport->width - 1.0f;
}

/*
 * Function: viewport_calc_element_top_ndc
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
float viewport_calc_element_top_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int top = viewport_calc_element_top_px(viewport, element);

    return (-2.0f *  top) / viewport->height + 1.0f;
}

/*
 * Function: viewport_calc_element_bottom_ndc
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
float viewport_calc_element_bottom_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int bottom_offset = viewport_calc_element_top_px(viewport, element) + viewport_calc_element_height_px(viewport, element);

    return (-2.0f *  bottom_offset) / viewport->height + 1.0f;
}

/*
 * Function: viewport_calc_element_z_index_ndc
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
float viewport_calc_element_z_index_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    float zIndexf = ((float)element->style.zIndex - GKIT_Z_INDEX_MIN) / (GKIT_Z_INDEX_MAX - GKIT_Z_INDEX_MIN);

    zIndexf = (zIndexf * (-1.0f - 1.0f)) + 1.0f;

    float pzi = element->parent
                    ? viewport_calc_element_z_index_ndc(viewport, element->parent)
                    : 1.0f; // Farther distance

    // Minimum depth is parent's depth
    return pzi < zIndexf ? pzi : zIndexf;
}

float viewport_calc_x_value_ndc(struct GKitViewport *viewport, unsigned int value)
{
    return (2.0f *  value) / viewport->width - 1.0f;
}

float viewport_calc_y_value_ndc(struct GKitViewport *viewport, unsigned int value)
{
    return (2.0f *  value) / viewport->height - 1.0f;
}
