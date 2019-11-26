#include "none.h"
#include "../internal/element.h"


int gkit_layout_none_element_width(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.layout.width.unit == GKIT_UNIT_PIXEL)
        return element->style.layout.width.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.layout.width.value.percentage / 100;

    return gkit_layout_element_width(element->parent, viewport) * element->style.layout.width.value.percentage / 100;
}

int gkit_layout_none_element_height(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.layout.height.unit == GKIT_UNIT_PIXEL)
        return element->style.layout.height.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.layout.height.value.percentage / 100;

    return gkit_layout_element_height(element->parent, viewport) * element->style.layout.height.value.percentage / 100;
}


int gkit_layout_none_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (element->style.layout.position.left.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.layout.position.left.value.pixels;

        return gkit_layout_element_left(element->parent, viewport) + element->style.layout.position.left.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.layout.position.left.value.percentage / 100;

    float calculated_left = gkit_layout_element_width(element->parent, viewport) * element->style.layout.position.left.value.percentage / 100;

    return gkit_layout_element_left(element->parent, viewport) + calculated_left;
}

int gkit_layout_none_element_right(struct GKitElement *element, struct GKitViewport *viewport)
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
    return gkit_layout_element_left(element, viewport) + gkit_layout_element_width(element, viewport);
}

int gkit_layout_none_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (element->style.layout.position.top.unit == GKIT_UNIT_PIXEL)
    {
        if (!element->parent)
            return element->style.layout.position.top.value.pixels;

        return gkit_layout_element_top(element->parent, viewport) + element->style.layout.position.top.value.pixels;
    }

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.layout.position.top.value.percentage / 100;

    float calculated_top = gkit_layout_element_height(element->parent, viewport) * element->style.layout.position.top.value.percentage / 100;

    return gkit_layout_element_top(element->parent, viewport) + calculated_top;
}

int gkit_layout_none_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    // We first get the bottom offset using the top and height:
    //
    //                 .--   +------------------------------------------------------------+
    //                 |     |                                                            |
    //                 |     |             .--  +-----------+    <--- Element top         |
    //      Viewport   |     |    Element  |    |           |                             |
    //       height    |     |    height   |    |           |                             |
    //                 |     |             `--  +-----------+                             |
    //                 |     |                                                            |
    //                  `--  +------------------------------------------------------------+
    //
    // viewport->height - (element->top + element->height) == element->bottom
    //
    return gkit_layout_element_top(element, viewport) + gkit_layout_element_height(element, viewport);
}