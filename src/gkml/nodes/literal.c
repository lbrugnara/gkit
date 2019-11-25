#include "literal.h"

struct GkmlLiteralNode* gkml_node_literal_new(enum GkmlLiteralType type, enum GkmlUnit unit)
{
    struct GkmlLiteralNode *literal_node = fl_malloc(sizeof(struct GkmlLiteralNode));
    literal_node->base.type = GKML_NODE_LITERAL;
    literal_node->type = type;
    literal_node->unit = unit;

    return literal_node;
}

/*
 * Function: free_literal_node
 *  Frees the memory of a <struct GkmlLiteralNode> object
 *
 * Parameters:
 *  literal_node - Node object
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_node_literal_free(struct GkmlLiteralNode *literal)
{
    if (!literal)
        return;

    if (literal->type == GKML_LITERAL_STRING && literal->value.string)
        fl_free(literal->value.string);

    fl_free(literal);
}
