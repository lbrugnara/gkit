#include "center.h"
#include "../internal/element.h"


int gkit_layout_center_element_width(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.layout.width.unit == GKIT_UNIT_PIXEL)
        return element->style.layout.width.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->width * element->style.layout.width.value.percentage / 100;

    return gkit_layout_element_width(element->parent, viewport) * element->style.layout.width.value.percentage / 100;
}

int gkit_layout_center_element_height(GKitElement element, struct GKitViewport *viewport)
{
    if (element->style.layout.height.unit == GKIT_UNIT_PIXEL)
        return element->style.layout.height.value.pixels;

    // GKIT_UNIT_PERCENTAGE
    if (!element->parent)
        return viewport->height * element->style.layout.height.value.percentage / 100;

    return gkit_layout_element_height(element->parent, viewport) * element->style.layout.height.value.percentage / 100;
}


int gkit_layout_center_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_width = gkit_layout_element_width(element, viewport);

    return (gkit_layout_element_width(element->parent, viewport) - element_width) / 2;
}

int gkit_layout_center_element_right(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_width = gkit_layout_element_width(element, viewport);

    return (gkit_layout_element_width(element->parent, viewport) - element_width) / 2;
}

int gkit_layout_center_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_height = gkit_layout_element_height(element, viewport);

    return (gkit_layout_element_height(element->parent, viewport) - element_height) / 2;
}

int gkit_layout_center_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_height = gkit_layout_element_height(element, viewport);

    return (gkit_layout_element_height(element->parent, viewport) - element_height) / 2;
}
