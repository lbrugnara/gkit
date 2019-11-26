#include "../../model/internal/element.h"
#include "../../model/layout.h"
#include "../../model/viewport.h"

/*
 * Function: gkit_calc_element_left_ndc
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
float gkit_calc_element_left_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    float left = gkit_layout_element_left(element, viewport);

    return (2.0f *  left) / viewport->width - 1.0f;
}

/*
 * Function: gkit_calc_element_right_ndc
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
float gkit_calc_element_right_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int right_offset = gkit_layout_element_left(element, viewport) + gkit_layout_element_width(element, viewport);

    return (2.0f *  right_offset) / viewport->width - 1.0f;
}

/*
 * Function: gkit_calc_element_top_ndc
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
float gkit_calc_element_top_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int top = gkit_layout_element_top(element, viewport);

    return (-2.0f *  top) / viewport->height + 1.0f;
}

/*
 * Function: gkit_calc_element_bottom_ndc
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
float gkit_calc_element_bottom_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    int bottom_offset = gkit_layout_element_top(element, viewport) + gkit_layout_element_height(element, viewport);

    return (-2.0f *  bottom_offset) / viewport->height + 1.0f;
}

/*
 * Function: gkit_calc_element_z_index_ndc
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
float gkit_calc_element_z_index_ndc(struct GKitViewport *viewport, struct GKitElement *element)
{
    float zIndexf = ((float)element->style.zIndex - GKIT_Z_INDEX_MIN) / (GKIT_Z_INDEX_MAX - GKIT_Z_INDEX_MIN);

    zIndexf = (zIndexf * (-1.0f - 1.0f)) + 1.0f;

    float pzi = element->parent
                    ? gkit_calc_element_z_index_ndc(viewport, element->parent)
                    : 1.0f; // Farther distance

    // Minimum depth is parent's depth
    return pzi < zIndexf ? pzi : zIndexf;
}

float gkit_calc_x_value_ndc(struct GKitViewport *viewport, int value)
{
    return (2.0f *  value) / viewport->width - 1.0f;
}

float gkit_calc_y_value_ndc(struct GKitViewport *viewport, int value)
{
    return (-2.0f *  value) / viewport->height + 1.0f;
}
