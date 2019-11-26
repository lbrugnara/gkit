#ifndef GKIT_MODEL_TEXT_H
#define GKIT_MODEL_TEXT_H

#include <stdbool.h>
#include "element.h"

typedef struct GKitElementText* GKitElementText;

/*
 * Function: gkit_text_create
 *  Creates a new <struct GKitElementText> object
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  <GKitElementText> - Text object
 *
 */
GKitElementText gkit_text_create(void);

/*
 * Function: gkit_text_draw
 *  Draws a text object into the current window.
 *
 * Parameters:
 *  <GKitElementText> element - Text to be drawn into the window
 *  <struct GKitViewport> viewport - Viewport's object
 *  <enum GKitElementType> type - Type of objects that should be drawn
 *
 * Returns:
 *  bool - *true* if the text can be drawn without errors, otherwise *false*.
 *
 */
bool gkit_text_draw(GKitElementText element, struct GKitViewport viewport, enum GKitElementType type);

/*
 * Function: gkit_text_destroy
 *  Frees the resources allocated within the <GKitElementText> instance.
 *
 * Parameters:
 *  element - <GKitElementText> to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_text_destroy(GKitElementText element);

#endif /* GKIT_MODEL_TEXT_H */
