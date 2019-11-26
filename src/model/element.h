#ifndef GKIT_MODEL_ELEMENT_H
#define GKIT_MODEL_ELEMENT_H

#include <stdbool.h>
#include "viewport.h"

enum GKitElementType {
    GKIT_ELEMENT_RECT,
    GKIT_ELEMENT_TEXT,
};

/*
 * Type: GKitElement
 *  A pointer to a <struct GKitElement>
 */
typedef struct GKitElement *GKitElement;

/*
 * Function: gkit_element_create
 *  Creates a new instance of a <GKitElement> based on the provided
 *  <GKitElementType>.
 *
 * Parameters:
 *  <enum GKitElementType> type - The type of element to be created
 *
 * Returns:
 *  GKitElement - Instance of the new <GKitElement>
 *
 * Notes:
 *  The objects returned by this function can be safely casted to the
 *  specific type of object they represent based on the <enum GKitElementType>
 */
GKitElement gkit_element_create(enum GKitElementType type);

/*
 * Function: gkit_element_add_child
 *  Adds a <GKitElement> as a *child* of another <GKitElement> (the *parent*).
 *
 * Parameters:
 *  parent - Parent <GKitElement> that will contain a new child element.
 *  child - <GKitElement> to be added as a new child of the parent element.
 *
 * Returns:
 *  bool - *true* if the parent object is a valid container and the child
 *          element can be added to it, otherwise *false*.
 */
bool gkit_element_add_child(GKitElement parent, GKitElement child);

/*
 * Function: gkit_element_draw
 *  If the *element*'s type is equals to the provided <enum GKitElementType> *type*, 
 *  this function draws the <GKitElement> to the current attached <GKitWindow>, otherwise
 *  the element is not rendered
 *
 * Parameters:
 *  <GKitElement> element - Element to be drawn.
 *  <struct GKitViewport> viewport - Object containing the viewport's properties.
 *  <enum GKitElementType> type - Type of elements to draw
 *
 * Returns:
 *  bool - *true* on success. Otherwise it returns *false*.
 * 
 * See Also:
 *  <gkit_window_make_current>
 */
bool gkit_element_draw(GKitElement element, struct GKitViewport viewport, enum GKitElementType type);

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

#endif /* GKIT_MODEL_ELEMENT_H */
