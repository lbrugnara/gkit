#include "style.h"

struct GkmlStyleNode* gkml_node_style_new(void)
{
    struct GkmlStyleNode *style = fl_malloc(sizeof(struct GkmlStyleNode));
    style->base.type = GKML_NODE_STYLE;
    style->attributes = gkml_node_attribute_map_new();
    return style;
}

void gkml_node_style_free(struct GkmlStyleNode *style)
{
    if (!style)
        return;

    if (style->attributes.map)
        gkml_node_attribute_map_free(&style->attributes);

    fl_free(style);
}
