#include "attribute.h"
#include "element.h"
#include "identifier.h"
#include "literal.h"
#include "style.h"

/*
 * Function: gkml_node_free
 *  Checks the node's *type* property to call the specific
 *  function that releases its memory.
 */
void gkml_node_free(struct GkmlNode *node)
{
    if (!node)
        return;

    if (node->type == GKML_NODE_LITERAL)
    {
        gkml_node_literal_free((struct GkmlLiteralNode*)node);
    }
    else if (node->type == GKML_NODE_ELEMENT)
    {
        gkml_node_element_free((struct GkmlElementNode*)node);
    }
    else if (node->type == GKML_NODE_STYLE)
    {
        gkml_node_style_free((struct GkmlStyleNode*)node);
    }
    else if (node->type == GKML_NODE_IDENTIFIER)
    {
        gkml_node_id_free((struct GkmlIdentifierNode*)node);
    }
    else if (node->type == GKML_NODE_ATTRIBUTE)
    {
        gkml_node_attribute_free((struct GkmlAttributeNode*)node);
    }
}
