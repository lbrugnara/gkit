#ifndef GKML_AST_NODE_H
#define GKML_AST_NODE_H

#include "../token.h"

/*
 * Enum: enum GkmlNodeType 
 *  Types of AST node
 */
enum GkmlNodeType {
    GKML_NODE_ELEMENT,
    GKML_NODE_STYLE,
    GKML_NODE_LITERAL,
    GKML_NODE_IDENTIFIER,
    GKML_NODE_ATTRIBUTE
};

/*
 * Struct: struct GkmlNode
 *  The base node object. All the specific objects are compound with this one
 * 
 * Members:
 *  <enum GkmlNodeType> type: The specific type of the AST node
 *  <struct GkmlSourceLocation> location: The place in the source code represented by the node
 *  <struct GkmlTypeInfo> typeinfo: The type of the operand or operator represented by the node
 */
struct GkmlNode {
    enum GkmlNodeType type;
    struct GkmlSourceLocation location;
};

/*
 * Function: gkml_node_free
 *  This function releases the memory allocated in the *node* object
 *  independently of the type of node
 *
 * Parameters:
 *  node - Node object to free
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_node_free(struct GkmlNode *node);

#endif /* GKML_AST_NODE_H */
