#ifndef GKML_AST_LITERAL_H
#define GKML_AST_LITERAL_H

#include "node.h"

enum GkmlLiteralType {
    GKML_LITERAL_INTEGER,
    GKML_LITERAL_STRING
};

enum GkmlUnit {
    GKML_UNIT_NONE,
    GKML_UNIT_PIXEL,
    GKML_UNIT_PERCENTAGE,
};

/*
 * Struct: struct GkmlLiteralNode
 *  Represents a literal value in the program's source.
 * 
 * Members:
 *  <struct GkmlNode> base: Basic information of the node object 
 *  <anonymous union> value: Based on the type information the union is populated with the C value
 * 
 */
struct GkmlLiteralNode {
    struct GkmlNode base;
    enum GkmlLiteralType type;
    enum GkmlUnit unit;
    union {
        uint32_t integer;
        char *string;
    } value;
};

struct GkmlLiteralNode* gkml_node_literal_new(enum GkmlLiteralType type, enum GkmlUnit unit);
void gkml_node_literal_free(struct GkmlLiteralNode *literal);

#endif /* GKML_AST_LITERAL_H */
