#ifndef GKIT_LAYOUT_CENTER_H
#define GKIT_LAYOUT_CENTER_H

#include "../element.h"
#include "../viewport.h"

int gkit_layout_center_element_width(GKitElement element, struct GKitViewport *viewport);
int gkit_layout_center_element_height(GKitElement element, struct GKitViewport *viewport);

int gkit_layout_center_element_left(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_center_element_right(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_center_element_top(struct GKitElement *element, struct GKitViewport *viewport);
int gkit_layout_center_element_bottom(struct GKitElement *element, struct GKitViewport *viewport);

#endif /* GKIT_LAYOUT_CENTER_H */
