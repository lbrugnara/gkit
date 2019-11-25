#ifndef GKML_AST_STYLE_H
#define GKML_AST_STYLE_H

#include "node.h"
#include "attribute.h"

struct GkmlStyleNode {
    struct GkmlNode base;
    struct GkmlAttributeNodeMap attributes;
};

struct GkmlStyleNode* gkml_node_style_new(void);
void gkml_node_style_free(struct GkmlStyleNode *style);

#endif /* GKML_AST_STYLE_H */
