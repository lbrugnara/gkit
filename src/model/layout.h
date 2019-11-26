#ifndef GKIT_MODEL_LAYOUT_H
#define GKIT_MODEL_LAYOUT_H

#include "element.h"
#include "viewport.h"

/*
 * Enum: GKitUnit
 *  A unit measurement to be used to calculate different properties
 *  of the elements to render to the screen
 * 
 */
enum GKitUnit {
    GKIT_UNIT_NONE,
    GKIT_UNIT_PIXEL,
    GKIT_UNIT_PERCENTAGE
};

enum GKitOverflow {
    GKIT_OVERFLOW_VISIBLE,
    GKIT_OVERFLOW_HIDDEN,
};

/*
 * Type: struct GKitValue
 *  Represents a unit of measurement and its associated value
 * 
 */
struct GKitValue {
    union {
        float percentage;
        unsigned int pixels;
    } value;
    enum GKitUnit unit;
};

enum GKitLayoutType {
    GKIT_LAYOUT_NONE,
    GKIT_LAYOUT_CENTER,
};

struct GKitLayout {
    enum GKitLayoutType type;

    struct GKitValue width;
    struct GKitValue height;

    enum GKitOverflow overflow;

    struct {
        struct GKitValue left;
        struct GKitValue right;
        struct GKitValue top;
        struct GKitValue bottom;
    } position;

    struct {
        struct GKitValue left;
        struct GKitValue right;
        struct GKitValue top;
        struct GKitValue bottom;
    } margin;
};

int gkit_layout_element_width(GKitElement element, struct GKitViewport *viewport);
int gkit_layout_element_height(GKitElement element, struct GKitViewport *viewport);

int gkit_layout_element_left(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_right(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_top(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_bottom(struct GKitElement *element, struct GKitViewport *viewport);

int gkit_layout_element_limit_left(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_limit_right(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_limit_top(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_element_limit_bottom(struct GKitElement *element, struct GKitViewport *viewport);

#endif /* GKIT_MODEL_LAYOUT_H */
