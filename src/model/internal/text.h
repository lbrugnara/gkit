#ifndef GKIT_INTERNAL_TEXT_H
#define GKIT_INTERNAL_TEXT_H

#include <stdbool.h>
#include "element.h"
#include "../text.h"
#include "../viewport.h"

struct GKitElementText {
    struct GKitElement base;
    char *content;
};

/*
 * Function: gkit_internal_text_create
 *  Creates a new instance of a <GKitElement>
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  GKitElement - Instance of the new <GKitElement>
 *
 */
bool gkit_internal_text_create(struct GKitElementText *element);

/*
 * Function: gkit_internal_text_draw
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
bool gkit_internal_text_draw(struct GKitElementText *element, struct GKitViewport viewport, enum GKitElementType type);

/*
 * Function: gkit_internal_text_destroy
 *  Frees the resources allocated within the <GKitElement> instance.
 *
 * Parameters:
 *  element - <GKitElement> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_internal_text_destroy(struct GKitElementText *element);

#endif /* GKIT_INTERNAL_TEXT_H */
