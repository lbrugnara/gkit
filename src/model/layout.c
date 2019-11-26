#include "layout.h"
#include "internal/element.h"
#include "layouts/none.h"
#include "layouts/center.h"


int gkit_layout_element_width(GKitElement element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_width(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_width(element, viewport);
    }

    return 0;
}

int gkit_layout_element_height(GKitElement element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_height(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_height(element, viewport);
    }

    return 0;
}


int gkit_layout_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_left(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_left(element, viewport);
    }

    return 0;
}

int gkit_layout_element_right(struct GKitElement *element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_right(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_right(element, viewport);
    }

    return 0;
}

int gkit_layout_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_top(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_top(element, viewport);
    }

    return 0;
}

int gkit_layout_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    switch (element->style.layout.type)
    {
        case GKIT_LAYOUT_NONE:
            return gkit_layout_none_element_bottom(element, viewport);

        case GKIT_LAYOUT_CENTER:
            return gkit_layout_center_element_bottom(element, viewport);
    }

    return 0;
}
