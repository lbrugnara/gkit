#include "layout.h"
#include "internal/element.h"
#include "layouts/none.h"
#include "layouts/center.h"
#include "layouts/mosaic.h"


int gkit_layout_element_width(GKitElement element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_width(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_width(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_width(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_width(element, viewport);
    }

    return 0;
}

int gkit_layout_element_height(GKitElement element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_height(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_height(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_height(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_height(element, viewport);
    }

    return 0;
}


int gkit_layout_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_left(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_left(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_left(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_left(element, viewport);
    }

    return 0;
}

int gkit_layout_element_right(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_right(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_right(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_right(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_right(element, viewport);
    }

    return 0;
}

int gkit_layout_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_top(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_top(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_top(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_top(element, viewport);
    }

    return 0;
}

int gkit_layout_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return gkit_layout_none_element_bottom(element, viewport);

    switch (element->parent->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_bottom(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_bottom(element, viewport);

        case GKIT_LAYOUT_MOSAIC:
            return gkit_layout_mosaic_element_bottom(element, viewport);
    }

    return 0;
}

int gkit_layout_element_limit_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    // If in the parent element the overflow is visible, we directly return
    // the parent's left limit.
    if (element->parent->style.layout.overflow == GKIT_OVERFLOW_VISIBLE)
        return gkit_layout_element_limit_left(element->parent, viewport);

    // Otherwise, the parent has overflow == hidden, in that case we need to
    // get the parent's left, which is the element's limit
    return gkit_layout_element_left(element->parent, viewport);
}

int gkit_layout_element_limit_right(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return viewport->width;

    // If in the parent element the overflow is visible, we directly return
    // the parent's right limit.
    if (element->parent->style.layout.overflow == GKIT_OVERFLOW_VISIBLE)
        return gkit_layout_element_limit_right(element->parent, viewport);

    // Otherwise, the parent has overflow == hidden, in that case we need to
    // get the parent's right, which is the element's limit
    return gkit_layout_element_right(element->parent, viewport);
}

int gkit_layout_element_limit_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    // If in the parent element the overflow is visible, we directly return
    // the parent's top limit.
    if (element->parent->style.layout.overflow == GKIT_OVERFLOW_VISIBLE)
        return gkit_layout_element_limit_top(element->parent, viewport);

    // Otherwise, the parent has overflow == hidden, in that case we need to
    // get the parent's top, which is the element's limit
    return gkit_layout_element_top(element->parent, viewport);
}

int gkit_layout_element_limit_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return viewport->height;

    // If in the parent element the overflow is visible, we directly return
    // the parent's bottom limit.
    if (element->parent->style.layout.overflow == GKIT_OVERFLOW_VISIBLE)
        return gkit_layout_element_limit_bottom(element->parent, viewport);

    // Otherwise, the parent has overflow == hidden, in that case we need to
    // get the parent's bottom, which is the element's limit
    return gkit_layout_element_bottom(element->parent, viewport);
}
