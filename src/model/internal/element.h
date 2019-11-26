#ifndef GKIT_INTERNAL_ELEMENT_H
#define GKIT_INTERNAL_ELEMENT_H

#include <stdbool.h>
#include "../element.h"
#include "../style.h"

/*
 * Type: struct GKitElement
 *  Represents an element that can be drawn into a <GKitWindow> instance
 *
 */
struct GKitElement {
    enum GKitElementType type;
    void *raw;
    struct GKitStyle style;
    struct GKitElement *parent;
};

#endif /* GKIT_INTERNAL_ELEMENT_H */
