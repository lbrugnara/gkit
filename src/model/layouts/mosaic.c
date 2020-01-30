#include "mosaic.h"
#include "../internal/element.h"
#include "../internal/text.h"


int gkit_layout_mosaic_element_width(GKitElement element, struct GKitViewport *viewport)
{
    if (element->type == GKIT_ELEMENT_RECT)
    {
        if (element->style.layout.width.unit == GKIT_UNIT_PIXEL)
            return element->style.layout.width.value.pixels;

        // GKIT_UNIT_PERCENTAGE
        if (!element->parent)
            return viewport->width * element->style.layout.width.value.percentage / 100;

        return gkit_layout_element_width(element->parent, viewport) * element->style.layout.width.value.percentage / 100;
    }

    if (element->type == GKIT_ELEMENT_TEXT)
    {
        GKitElementText text = (GKitElementText)element;
        // FIXME: We need the font-size to scale this value

        if (text->content == NULL)
            return 0;

        int width = 0;
        for (size_t i=0; i < strlen(text->content); i++)
        {
            struct GKitCharacter character = text->font->alphabet[(int)text->content[i]];
            // The advance property is the distance between the begining of the different
            // characters
            width += character.advance >> 6;
        }

        return width;
    }

    return 0;
}

int gkit_layout_mosaic_element_height(GKitElement element, struct GKitViewport *viewport)
{
    if (element->type == GKIT_ELEMENT_RECT)
    {
        if (element->style.layout.height.unit == GKIT_UNIT_PIXEL)
            return element->style.layout.height.value.pixels;

        // GKIT_UNIT_PERCENTAGE
        if (!element->parent)
            return viewport->height * element->style.layout.height.value.percentage / 100;

        return gkit_layout_element_height(element->parent, viewport) * element->style.layout.height.value.percentage / 100;
    }
    
    if (element->type == GKIT_ELEMENT_TEXT)
    {
        GKitElementText text = (GKitElementText)element;
        // FIXME: We need the font-size to scale this value
        return text->font->gt_height;
    }

    return 0;
}


int gkit_layout_mosaic_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    // Root element
    if (!element->parent)
    {
        return element->style.layout.position.left.unit == GKIT_UNIT_PIXEL 
                ? element->style.layout.position.left.value.pixels
                : viewport->width * element->style.layout.position.left.value.percentage / 100;
    }

    // rest of the elements...

    // If the element is the first child, we start at the parent's left, otherwise
    // we start at the previous sibling's right side
    int left_offset = element->previous 
                        ? gkit_layout_element_right(element->previous, viewport)
                        : gkit_layout_element_left(element->parent, viewport);

    int element_left = 0;

    if (element->style.layout.position.left.unit == GKIT_UNIT_PIXEL)
    {
        // For pixels we just add the element's left value to the left offset we calculated above
        element_left = left_offset + element->style.layout.position.left.value.pixels;
    }
    else
    {
        // For percentage we calculate it based on the parent's width and then we add the left offset calculated above
        float calculated_left = gkit_layout_element_width(element->parent, viewport) * element->style.layout.position.left.value.percentage / 100;
        element_left = left_offset + calculated_left;
    }

    if (gkit_layout_element_width(element, viewport) + element_left > (int)viewport->width)
        return 0;

    return element_left;
}

int gkit_layout_mosaic_element_right(struct GKitElement *element, struct GKitViewport *viewport)
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

int gkit_layout_mosaic_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    // Root element
    if (!element->parent)
    {
        return element->style.layout.position.top.unit == GKIT_UNIT_PIXEL 
                ? element->style.layout.position.top.value.pixels
                : viewport->width * element->style.layout.position.top.value.percentage / 100;
    }

    // rest of the elements...
    int element_top = 0;
    int element_calculated_top = 0;
    int element_width = gkit_layout_element_width(element, viewport);
    int parent_element_top = gkit_layout_element_top(element->parent, viewport);    

    if (element->style.layout.position.top.unit == GKIT_UNIT_PIXEL)
    {
        element_top = element->style.layout.position.top.value.pixels;
    }
    else
    {
        element_top = gkit_layout_element_height(element->parent, viewport) * element->style.layout.position.top.value.percentage / 100;
    }

    element_calculated_top = element_top + parent_element_top;

    if (element->previous && element_width + gkit_layout_element_right(element->previous, viewport) > (int)viewport->width)
    {
        GKitElement sibling = element->previous;
        int bottom = 0;
        do {
            int sibling_bottom = gkit_layout_element_bottom(sibling, viewport);
            if (element_width <= gkit_layout_element_width(sibling, viewport) || sibling_bottom > bottom)
                bottom = sibling_bottom;
            sibling = sibling->previous;
        } while (sibling);

        return bottom;
    }

    return element_calculated_top;
}

int gkit_layout_mosaic_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
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
