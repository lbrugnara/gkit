#include <stdio.h>
#include "build.h"
#include "context.h"
#include "parse.h"
#include "../model/internal/rect.h"
#include "../model/internal/text.h"

#define MAP_START
#define MAP_END map_end:
#define map_literal_handler(attr, prop, func, pointer)                       \
    if (flm_cstring_equals((attr)->name, (prop))) {                             \
        func(ctx, (struct GkmlLiteralNode*)(attr)->value, (prop), (pointer));   \
        goto map_end;                                                           \
    }

GKitElement gkml_visit_text_element_node(struct GkmlContext *ctx, struct GkmlTextElementNode *node);
GKitElement gkml_visit_rect_element_node(struct GkmlContext *ctx, struct GkmlRectElementNode *node);
GKitElement gkml_visit_element_node(struct GkmlContext *ctx, struct GkmlElementNode *node);
void gkml_visit_style_node(struct GkmlContext *ctx, struct GkmlStyleNode *node, struct GKitStyle *style);

static void literal_to_gkit_color(struct GkmlContext *ctx, struct GkmlLiteralNode *literal, const char *attr_name, struct GKitColor *color)
{
    if (literal->type != GKML_LITERAL_INTEGER)
    {
        gkml_context_error(ctx, literal->base.location, GKML_ERROR_TYPE_MISSMATCH, "Expecting an integer value for property '%s'", attr_name);
        return;
    }

    color->red = 0xFF & (literal->value.integer >> 16);
    color->green = 0xFF & (literal->value.integer >> 8);
    color->blue = 0xFF & literal->value.integer;
    // FIXME: Add alpha channel
    color->alpha = 1.0f;
}

static void literal_to_gkit_value(struct GkmlContext *ctx, struct GkmlLiteralNode *literal, const char *attr_name, struct GKitValue *unit)
{
    if (literal->type != GKML_LITERAL_INTEGER)
    {
        gkml_context_error(ctx, literal->base.location, GKML_ERROR_TYPE_MISSMATCH, "Expecting an integer value for property '%s'", attr_name);
        return;
    }

    switch (literal->unit)
    {
        case GKML_UNIT_NONE:
            unit->unit = GKIT_UNIT_NONE;
            gkml_context_error(ctx, literal->base.location, GKML_ERROR_TYPE_MISSMATCH, "Missing unit for '%s' property, please use 'px' or '%%'", attr_name);
            break;
        case GKML_UNIT_PIXEL:
            unit->unit = GKIT_UNIT_PIXEL;
            unit->value.pixels = literal->value.integer;
            break;
        case GKML_UNIT_PERCENTAGE:
            unit->unit = GKIT_UNIT_PERCENTAGE;
            unit->value.percentage = literal->value.integer;
            break;
    }
}

static void literal_to_integer(struct GkmlContext *ctx, struct GkmlLiteralNode *literal, const char *attr_name, int *value)
{
    if (literal->type != GKML_LITERAL_INTEGER)
    {
        gkml_context_error(ctx, literal->base.location, GKML_ERROR_TYPE_MISSMATCH, "Expecting an integer value for property '%s'", attr_name);
        return;
    }

    *value = literal->value.integer;
}

static void string_to_gkit_layout_type(struct GkmlContext *ctx, struct GkmlLiteralNode *literal, const char *attr_name, enum GKitLayoutType *value)
{
    if (literal->type != GKML_LITERAL_STRING)
    {
        gkml_context_error(ctx, literal->base.location, GKML_ERROR_TYPE_MISSMATCH, "Expecting a string value for property '%s'", attr_name);
        return;
    }

    if (flm_cstring_equals(literal->value.string, "none"))
    {
        *value = GKIT_LAYOUT_NONE;
    }
    else if (flm_cstring_equals(literal->value.string, "center"))
    {
        *value = GKIT_LAYOUT_CENTER;
    }
    else
    {
        gkml_context_error(ctx, literal->base.location, GKML_ERROR_UNKNOWN_VALUE, "Unknown value '%s' for property '%s'", literal->value.string, attr_name);
    }
}

void gkml_visit_style_node(struct GkmlContext *ctx, struct GkmlStyleNode *node, struct GKitStyle *style)
{
    if (!node)
        return;

    const char **node_attrs = gkml_node_attribute_map_keys(&node->attributes);

    for (size_t i=0; i < fl_array_length(node_attrs); i++)
    {
        struct GkmlAttributeNode *attr = gkml_node_attribute_map_get(&node->attributes, node_attrs[i]);

        if (attr == NULL)
            continue;

        MAP_START;
        map_literal_handler(attr, "color", literal_to_gkit_color, &style->color);
        map_literal_handler(attr, "layout-type", string_to_gkit_layout_type, &style->layout.type);
        map_literal_handler(attr, "position-left", literal_to_gkit_value, &style->layout.position.left);
        map_literal_handler(attr, "position-right", literal_to_gkit_value, &style->layout.position.right);
        map_literal_handler(attr, "position-top", literal_to_gkit_value, &style->layout.position.top);
        map_literal_handler(attr, "position-bottom", literal_to_gkit_value, &style->layout.position.bottom);
        map_literal_handler(attr, "margin-left", literal_to_gkit_value, &style->layout.margin.left);
        map_literal_handler(attr, "margin-right", literal_to_gkit_value, &style->layout.margin.right);
        map_literal_handler(attr, "margin-top", literal_to_gkit_value, &style->layout.margin.top);
        map_literal_handler(attr, "margin-bottom", literal_to_gkit_value, &style->layout.margin.bottom);
        map_literal_handler(attr, "width", literal_to_gkit_value, &style->layout.width);
        map_literal_handler(attr, "height", literal_to_gkit_value, &style->layout.height);
        map_literal_handler(attr, "z-index", literal_to_integer, &style->zIndex);
        MAP_END;
    }

    fl_array_free(node_attrs);
}

GKitElement gkml_visit_text_element_node(struct GkmlContext *ctx, struct GkmlTextElementNode *node)
{
    struct GKitElementText *element = (struct GKitElementText*)gkit_element_create(GKIT_ELEMENT_TEXT);
    
    gkml_visit_style_node(ctx, node->base_element.style, &element->base.style);

    element->content = fl_cstring_new(0);

    for (size_t i=0; i < fl_array_length(node->content); i++)
    {
        struct GkmlLiteralNode *literal = (struct GkmlLiteralNode*)node->content[i];

        if (literal->type != GKML_LITERAL_STRING || !literal->value.string)
            continue;

        fl_cstring_append(&element->content, literal->value.string);
    }
    
    return (GKitElement)element;
}

GKitElement gkml_visit_rect_element_node(struct GkmlContext *ctx, struct GkmlRectElementNode *node)
{
    struct GKitElementRect *element = (struct GKitElementRect*)gkit_element_create(GKIT_ELEMENT_RECT);
    
    gkml_visit_style_node(ctx, node->base_element.style, &element->base.style);

    if (node->children)
    {
        for (size_t i=0; i < fl_array_length(node->children); i++)
        {
            // FIXME: add visit_node or change children's type
            GKitElement child = gkml_visit_element_node(ctx, (struct GkmlElementNode*)node->children[i]);

            if (child == NULL)
                continue;

            gkit_element_add_child((GKitElement)element, child);
        }
    }

    return (GKitElement)element;
}

GKitElement gkml_visit_element_node(struct GkmlContext *ctx, struct GkmlElementNode *node)
{
    if (node == NULL)
        return NULL;

    if (node->type == GKML_ELEMENT_RECT || node->type == GKML_ELEMENT_ROOT)
        return gkml_visit_rect_element_node(ctx, (struct GkmlRectElementNode*)node);
    
    if (node->type == GKML_ELEMENT_TEXT)
        return gkml_visit_text_element_node(ctx, (struct GkmlTextElementNode*)node);
    
    gkml_context_error(ctx, node->base.location, GKML_ERROR_INTERNAL, "Element type not handled in visitor function");

    return NULL;
}

GKitElement gkml_build_tree(struct GkmlContext *ctx)
{
    if (!ctx || !ctx->ast || !ctx->ast->root)
        return NULL;

    return gkml_visit_rect_element_node(ctx, ctx->ast->root);
}
