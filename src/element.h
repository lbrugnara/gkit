#ifndef GKIT_ELEMENT_H
#define GKIT_ELEMENT_H

#include <fllib.h>
#include "internal/element.h"

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

#endif /* GKIT_ELEMENT_H */
