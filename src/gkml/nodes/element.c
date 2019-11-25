#include "element.h"

struct GkmlElementNode* gkml_node_element_new(enum GkmlElementType type)
{
    struct GkmlElementNode *element = NULL;

    switch (type)
    {
        case GKML_ELEMENT_ROOT:
        case GKML_ELEMENT_RECT:
        {
            struct GkmlRectElementNode *rect = fl_malloc(sizeof(struct GkmlRectElementNode));
            rect->children = fl_array_new(sizeof(struct GkmlNode*), 0);
            element = (struct GkmlElementNode*)rect;
            break;
        }
        case GKML_ELEMENT_TEXT:
        {
            struct GkmlTextElementNode *text = fl_malloc(sizeof(struct GkmlTextElementNode));
            text->content = fl_array_new(sizeof(struct GkmlNode*), 0);
            element = (struct GkmlElementNode*)text;
            break;
        }
        default:
            return NULL;
    }
        
    element->type = type;
    element->attributes = gkml_node_attribute_map_new();
    element->base.type = GKML_NODE_ELEMENT;

    return element;
}

void gkml_node_element_free(struct GkmlElementNode *element)
{
    if (!element)
        return;

    switch (element->type)
    {
        case GKML_ELEMENT_ROOT:
        case GKML_ELEMENT_RECT:
        {
            struct GkmlRectElementNode *rect = (struct GkmlRectElementNode*)element;
            fl_array_free_each_pointer(rect->children, (FlArrayFreeElementFunc)gkml_node_free);
            break;
        }
        case GKML_ELEMENT_TEXT:
        {
            struct GkmlTextElementNode *text = (struct GkmlTextElementNode*)element;
            fl_array_free_each_pointer(text->content, (FlArrayFreeElementFunc)gkml_node_free);
            break;
        }
        default:
            return;
    }

    if (element->style)
        gkml_node_free((struct GkmlNode*)element->style);

    if (element->attributes.map)
        gkml_node_attribute_map_free(&element->attributes);

    fl_free(element);
}
