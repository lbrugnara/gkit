#ifndef GKML_AST_ATTRIBUTE_H
#define GKML_AST_ATTRIBUTE_H

#include <fllib.h>

#include "node.h"

struct GkmlAttributeNode {
    struct GkmlNode base;
    char *name;
    struct GkmlNode *value;
};

struct GkmlAttributeNodeMap {
    FlHashtable *map;
};

static inline struct GkmlAttributeNode* gkml_node_attribute_new(char *name, struct GkmlNode *value)
{
    struct GkmlAttributeNode *attr = fl_malloc(sizeof(struct GkmlAttributeNode));
    attr->base.type = GKML_NODE_ATTRIBUTE;
    attr->name = name;
    attr->value = value;

    return attr;
}

static inline void gkml_node_attribute_free(struct GkmlAttributeNode *attribute_node)
{
    if (!attribute_node)
        return;

    if (attribute_node->name)
        fl_cstring_free(attribute_node->name);

    if (attribute_node->value)
        gkml_node_free(attribute_node->value);

    fl_free(attribute_node);
}

static inline struct GkmlAttributeNodeMap gkml_node_attribute_map_new()
{
    return (struct GkmlAttributeNodeMap) {
        .map = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_string,
            .key_allocator = fl_container_allocator_string,
            .key_comparer = fl_container_equals_string,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = (FlContainerCleanupFunction)gkml_node_attribute_free,
            .value_allocator = NULL
        })
    };
}

static inline void gkml_node_attribute_map_free(struct GkmlAttributeNodeMap *mapptr)
{
    fl_hashtable_free(mapptr->map);
}

static inline struct GkmlAttributeNode* gkml_node_attribute_map_add(struct GkmlAttributeNodeMap *mapptr, struct GkmlAttributeNode *attr)
{
    return (struct GkmlAttributeNode*)fl_hashtable_add(mapptr->map, attr->name, attr);
}

static inline const char** gkml_node_attribute_map_keys(struct GkmlAttributeNodeMap *mapptr)
{
    return fl_hashtable_keys(mapptr->map);
}

static inline struct GkmlAttributeNode** gkml_node_attribute_map_values(struct GkmlAttributeNodeMap *mapptr)
{
    return fl_hashtable_values(mapptr->map);
}

static inline struct GkmlAttributeNode* gkml_node_attribute_map_get(struct GkmlAttributeNodeMap *mapptr, const char *attrname)
{
    return (struct GkmlAttributeNode*)fl_hashtable_get(mapptr->map, attrname);
}

static inline size_t gkml_node_attribute_map_length(struct GkmlAttributeNodeMap *mapptr)
{
    return fl_hashtable_length(mapptr->map);
}

static inline bool gkml_node_attribute_map_has_key(struct GkmlAttributeNodeMap *mapptr, const char *attrname)
{
    return fl_hashtable_has_key(mapptr->map, attrname);
}

#endif /* GKML_AST_ATTRIBUTE_H */
