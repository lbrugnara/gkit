#ifndef GKIT_MODEL_STYLE_H
#define GKIT_MODEL_STYLE_H

#include "layout.h"

#define GKIT_Z_INDEX_MIN (-32768)
#define GKIT_Z_INDEX_MAX 32767

/*
 * Type: struct GKitColor
 *  Represents a RGBA color
 *
 */
struct GKitColor {
    float alpha;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

/*
 * Type: struct GKitStyle
 *  Represents the style of a <GKitElement>
 *
 */
struct GKitStyle {
    struct GKitColor color;
    struct GKitLayout layout;
    int zIndex;
};

#endif /* GKIT_MODEL_STYLE_H */
