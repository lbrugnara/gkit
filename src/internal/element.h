#ifndef GKIT_ELEMENT_INTERNAL_H
#define GKIT_ELEMENT_INTERNAL_H

/* About: Window internals
 *  The types and functions defined in this header are intended for private use only. Libraries referencing these
 *  types or functions directly will probably get breaking changes on newer versions.
 */

#include <fllib.h>

#define GKIT_Z_INDEX_MIN (-32768)
#define GKIT_Z_INDEX_MAX 32767

/*
 * Type: struct GKitViewport
 *  Represents the window viewport's size in pixels
 * 
 * ===== C ===== 
 *  struct GKitViewport  {
 *      unsigned int width;
 *      unsigned int height;
 *  };
 */
struct GKitViewport  {
    unsigned int width;
    unsigned int height;
};

/*
 * Type: struct GKitColor
 *  Represents a RGBA color
 *
 * ===== C ===== 
 *  struct GKitColor {
 *      float alpha;
 *      unsigned char red;
 *      unsigned char green;
 *      unsigned char blue;
 *  };
 * 
 */
struct GKitColor {
    float alpha;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

/*
 * Type: struct GKitUnit
 *  Represents a unit of measurement and its associated value:
 * 
 * ===== C =====
 *  // If the *unit* member is PIXEL it is safe to access the *value.pixels* members. When the
 *  // *unit* member is PERCENTAGE, the member *value.percentage* can be safely accessed.
 *  struct GKitUnit {
 *      union {
 *          float percentage;
 *          unsigned int pixels;
 *      } value;
 *      enum {
 *          GKIT_UNIT_PIXEL,
 *          GKIT_UNIT_PERCENTAGE
 *      } unit;
 *  };
 * 
 */
struct GKitUnit {
    union {
        float percentage;
        unsigned int pixels;
    } value;
    enum {
        GKIT_UNIT_PIXEL,
        GKIT_UNIT_PERCENTAGE
    } unit;
};

/*
 * Type: struct GKitStyle
 *  Represents the style of a <GKitElement>
 *
 * ===== C =====
 *  struct GKitStyle {
 *      struct GKitColor color;
 *      struct GKitUnit top;
 *      struct GKitUnit left;
 *      struct GKitUnit width;
 *      struct GKitUnit height;
 *      int zIndex;
 *  };
 *
 */
struct GKitStyle {
    struct GKitColor color;
    struct GKitUnit top;
    struct GKitUnit left;
    struct GKitUnit width;
    struct GKitUnit height;
    int zIndex;
};

enum GKitElementType {
    GKIT_ELEMENT_RECT,
    GKIT_ELEMENT_TEXT,
};

/*
 * Type: struct GKitElement
 *  Represents an element that can be drawn into a <GKitWindow> instance
 *
 * ===== C ===== 
 *  struct GKitElement {
 *      struct GKitStyle style;
 *      struct GKitElement *parent;
 *      FlList children;
 *  } *GKitElement;
 * 
 */
struct GKitElement {
    enum GKitElementType type;
    void *raw;
    struct GKitStyle style;
    struct GKitElement *parent;
};

struct GKitElementRect {
    struct GKitElement base;
    FlList children;
};

struct GKitElementText {
    struct GKitElement base;
    char *content;
};

/*
 * Function: gkit_internal_element_create
 *  Creates a new instance of a <GKitElement>
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  GKitElement - Instance of the new <GKitElement>
 *
 */
bool gkit_internal_element_create(struct GKitElement *element);

/*
 * Function: gkit_internal_element_draw
 *  Draws a <GKitElement> to the current attached <GKitWindow>.
 *
 * Parameters:
 *  element - <GKitElement> to be drawn.
 *  viewport - <GKitWindow> viewport's size.
 *
 * Returns:
 *  bool - *true* on success. Otherwise it returns *false*.
 * 
 * See Also:
 *  <gkit_window_make_current>
 */
bool gkit_internal_element_draw(struct GKitElement *element, struct GKitViewport viewport, enum GKitElementType type);

/*
 * Function: gkit_internal_element_destroy
 *  Frees the resources allocated within the <GKitElement> instance.
 *
 * Parameters:
 *  element - <GKitElement> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_internal_element_destroy(struct GKitElement *element);

#endif /* GKIT_ELEMENT_INTERNAL_H */
