#ifndef GKML_AST_ELEMENT_H
#define GKML_AST_ELEMENT_H

#include "node.h"
#include "attribute.h"

enum GkmlElementType {
    GKML_ELEMENT_ROOT,
    GKML_ELEMENT_RECT,
    GKML_ELEMENT_TEXT,
};

struct GkmlElementNode {
    struct GkmlNode base;    
    enum GkmlElementType type;
    struct GkmlStyleNode *style;
    struct GkmlAttributeNodeMap attributes;
};

struct GkmlRectElementNode {
    struct GkmlElementNode base_element;
    struct GkmlNode **children;
};

struct GkmlTextElementNode {
    struct GkmlElementNode base_element;
    struct GkmlNode **content;
};

struct GkmlElementNode* gkml_node_element_new(enum GkmlElementType type);
void gkml_node_element_free(struct GkmlElementNode *element);

#endif /* GKML_AST_ELEMENT_H */
