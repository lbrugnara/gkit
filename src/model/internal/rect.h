#ifndef GKIT_INTERNAL_RECT_H
#define GKIT_INTERNAL_RECT_H

#include <stdbool.h>
#include "element.h"
#include "../rect.h"
#include "../viewport.h"

struct GKitElementRect {
    struct GKitElement base;
    FlList *children;
};

/*
 * Function: gkit_internal_rect_create
 *  Creates a new instance of a <GKitElement>
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  GKitElement - Instance of the new <GKitElement>
 *
 */
bool gkit_internal_rect_create(struct GKitElementRect *element);

/*
 * Function: gkit_internal_rect_draw
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
bool gkit_internal_rect_draw(struct GKitElementRect *element, struct GKitViewport viewport, enum GKitElementType type);

/*
 * Function: gkit_internal_rect_destroy
 *  Frees the resources allocated within the <GKitElement> instance.
 *
 * Parameters:
 *  element - <GKitElement> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_internal_rect_destroy(struct GKitElementRect *element);

#endif /* GKIT_INTERNAL_RECT_H */
