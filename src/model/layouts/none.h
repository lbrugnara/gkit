#ifndef GKIT_LAYOUT_NONE_H
#define GKIT_LAYOUT_NONE_H

#include "../element.h"
#include "../viewport.h"

int gkit_layout_none_element_width(GKitElement element, struct GKitViewport *viewport);
int gkit_layout_none_element_height(GKitElement element, struct GKitViewport *viewport);

int gkit_layout_none_element_left(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_none_element_right(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_none_element_top(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_none_element_bottom(struct GKitElement *element, struct GKitViewport *viewport);

#endif /* GKIT_LAYOUT_NONE_H */
