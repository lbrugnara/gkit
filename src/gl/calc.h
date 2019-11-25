#ifndef GKIT_GL_VIEWPORT_H
#define GKIT_GL_VIEWPORT_H

#include "../internal/element.h"

int gkit_calc_element_width_px(struct GKitViewport *viewport, struct GKitElement *element);
int gkit_calc_element_height_px(struct GKitViewport *viewport, struct GKitElement *element);

int gkit_calc_element_left_px(struct GKitViewport *viewport, struct GKitElement *element);
int gkit_calc_element_right_px(struct GKitViewport *viewport, struct GKitElement *element);
int gkit_calc_element_top_px(struct GKitViewport *viewport, struct GKitElement *element);
int gkit_calc_element_bottom_px(struct GKitViewport *viewport, struct GKitElement *element);

float gkit_calc_element_left_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_right_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_top_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_bottom_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_z_index_ndc(struct GKitViewport *viewport, struct GKitElement *element);

float gkit_calc_x_value_ndc(struct GKitViewport *viewport, unsigned int value);
float gkit_calc_y_value_ndc(struct GKitViewport *viewport, unsigned int value);

#endif /* GKIT_GL_VIEWPORT_H */
