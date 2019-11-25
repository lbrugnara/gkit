#ifndef GKML_AST_IDENTIFIER_H
#define GKML_AST_IDENTIFIER_H

#include "node.h"

/*
 * Struct: struct GkmlIdentifierNode
 *  Represents a reference to an identifier
 */
struct GkmlIdentifierNode {
    struct GkmlNode base;
    char *name;
};

void gkml_node_id_free(struct GkmlIdentifierNode *id);

#endif /* GKML_AST_IDENTIFIER_H */
