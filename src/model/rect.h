#ifndef GKIT_MODEL_RECT_H
#define GKIT_MODEL_RECT_H

#include <stdbool.h>
#include "element.h"

typedef struct GKitElementRect* GKitElementRect;

/*
 * Function: gkit_rect_create
 *  Creates a new <struct GKitElementRect> object
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  <GKitElementRect> - Rectangle object
 *
 */
GKitElementRect gkit_rect_create(void);

/*
 * Function: gkit_rect_draw
 *  Draws a rectangle -and its children- into the current window.
 *
 * Parameters:
 *  <GKitElementRect> element - Rectangle to be drawn into the window
 *  <struct GKitViewport> viewport - Viewport's object
 *  <enum GKitElementType> type - Type of objects that should be drawn
 *
 * Returns:
 *  bool - *true* if the rectangle can be drawn without errors, otherwise *false*.
 *
 */
bool gkit_rect_draw(GKitElementRect element, struct GKitViewport viewport, enum GKitElementType type);

/*
 * Function: gkit_rect_destroy
 *  Frees the resources allocated within the <GKitElementRect> instance.
 *
 * Parameters:
 *  element - <GKitElementRect> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_rect_destroy(GKitElementRect element);

#endif /* GKIT_MODEL_RECT_H */
