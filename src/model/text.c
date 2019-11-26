#include <stdio.h>
#include <fllib.h>
#include "element.h"
#include "internal/text.h"

static inline void set_default_style(struct GKitStyle *style)
{
    style->color = (struct GKitColor) {
        .red = 0x0,
        .green = 0x0,
        .blue = 0x0,
        .alpha = 1.0f
    };

    style->layout = (struct GKitLayout) {
        .type = GKIT_LAYOUT_NONE,

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

GKitElementText gkit_text_create(void)
{
    struct GKitElementText *element = fl_malloc(sizeof(struct GKitElementText));
    element->base.type = GKIT_ELEMENT_TEXT;

    set_default_style(&element->base.style);
    
    if (!gkit_internal_text_create(element))
    {
        fl_free(element);
        return NULL;
    }

    return element;
}

bool gkit_text_draw(GKitElementText element, struct GKitViewport viewport, enum GKitElementType type)
{
    return gkit_internal_text_draw(element, viewport, type);
}

void gkit_text_destroy(GKitElementText text)
{
    gkit_internal_text_destroy(text);

    if (text->content)
        fl_cstring_free(text->content);

    fl_free(text);
}
