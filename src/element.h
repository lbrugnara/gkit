#ifndef GKIT_ELEMENT_H
#define GKIT_ELEMENT_H

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
struct  GKitElement {
    struct GKitStyle style;
    struct GKitElement *parent;
    FlList children;
};

/*
 * Type: GKitElement
 *  A pointer to a <struct GKitElement>
 */
typedef struct GKitElement *GKitElement;


/*
 * Function: gkit_element_create
 *  Creates a new instance of a <GKitElement>
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  GKitElement - Instance of the new <GKitElement>
 *
 */
GKitElement gkit_element_create(void);

/*
 * Function: gkit_element_add_child
 *  Adds a <GKitElement> as a *child* of another <GKitElement> (the *parent*).
 *
 * Parameters:
 *  parent - Parent <GKitElement> that will contain a new child element.
 *  child - <GKitElement> to be added as a new child of the parent element.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_element_add_child(GKitElement parent, GKitElement child);

/*
 * Function: gkit_element_draw
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
bool gkit_element_draw(GKitElement element, struct GKitViewport viewport);

/*
 * Function: gkit_element_destroy
 *  Frees the resources allocated within the <GKitElement> instance.
 *
 * Parameters:
 *  element - <GKitElement> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_element_destroy(GKitElement element);

#endif /* GKIT_ELEMENT_H */
