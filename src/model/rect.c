#include <stdio.h>
#include <fllib.h>
#include "element.h"
#include "internal/rect.h"

static inline void set_default_style(struct GKitStyle *style)
{
    style->color = (struct GKitColor) {
        .red = 0xFF,
        .green = 0xFF,
        .blue = 0xFF,
        .alpha = 1.0f
    };

    style->layout = (struct GKitLayout) {
        .type = GKIT_LAYOUT_NONE,

        .overflow = GKIT_OVERFLOW_VISIBLE,
        
        .width  = { .unit = GKIT_UNIT_PERCENTAGE, .value.percentage = 100 },
        .height = { .unit = GKIT_UNIT_PERCENTAGE, .value.percentage = 100 },

        .position = {
            .left   = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .right  = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .top    = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .bottom = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
        },

        .margin = {
            .left   = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .right  = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .top    = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .bottom = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
        }
    };

    style->zIndex = 0;
}

GKitElementRect gkit_rect_create(void)
{
    struct GKitElementRect *element = fl_malloc(sizeof(struct GKitElementRect));
    element->base.type = GKIT_ELEMENT_RECT;

    set_default_style(&element->base.style);

    if (!gkit_internal_rect_create(element))
    {
        fl_free(element);
        return NULL;
    }

    return element;
}

bool gkit_rect_draw(GKitElementRect rect, struct GKitViewport viewport, enum GKitElementType type)
{
    gkit_internal_rect_draw(rect, viewport, type);

    if (rect->children)
    {
        struct FlListNode *node = fl_list_head(rect->children);
        while (node) {
            gkit_element_draw((GKitElement)node->value, viewport, type);
            node = node->next;
        }
    }

    return true;
}

void gkit_rect_destroy(GKitElementRect rect)
{
    gkit_internal_rect_destroy(rect);

    if (rect->children)
    {
        struct FlListNode *node = fl_list_head(rect->children);
        while (node) 
        {
            gkit_element_destroy((GKitElement)node->value);
            node = node->next;
        }
        fl_list_free(rect->children);
    }

    fl_free(rect);
}
