#include "center.h"
#include "../internal/element.h"
#include "../internal/text.h"


int gkit_layout_center_element_width(GKitElement element, struct GKitViewport *viewport)
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
        // FIXME: We need the font-size to scale the value

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

int gkit_layout_center_element_height(GKitElement element, struct GKitViewport *viewport)
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
        // FIXME: We need the font-size to scale the value
        return text->font->gt_height;
    }

    return 0;
}


int gkit_layout_center_element_left(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_width = gkit_layout_element_width(element, viewport);
    int element_left = (gkit_layout_element_width(element->parent, viewport) - element_width) / 2;
    int parent_left = gkit_layout_element_left(element->parent, viewport);

    return parent_left + element_left;
}

int gkit_layout_center_element_right(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_width = gkit_layout_element_width(element, viewport);
    int element_right = (gkit_layout_element_width(element->parent, viewport) - element_width) / 2;
    int parent_right = gkit_layout_element_right(element->parent, viewport);

    return parent_right - element_right;
}

int gkit_layout_center_element_top(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_height = gkit_layout_element_height(element, viewport);
    int element_top = (gkit_layout_element_height(element->parent, viewport) - element_height) / 2;
    int parent_top = gkit_layout_element_top(element->parent, viewport);

    return parent_top + element_top;
}

int gkit_layout_center_element_bottom(struct GKitElement *element, struct GKitViewport *viewport)
{
    if (!element->parent)
        return 0;

    int element_height = gkit_layout_element_height(element, viewport);
    int element_bottom = (gkit_layout_element_height(element->parent, viewport) - element_height) / 2;
    int parent_bottom = gkit_layout_element_bottom(element->parent, viewport);

    return parent_bottom - element_bottom;
}
