#ifndef GKIT_GL_MODEL_CALC_H
#define GKIT_GL_MODEL_CALC_H

#include "../../model/element.h"
#include "../../model/viewport.h"

float gkit_calc_element_left_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_right_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_top_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_bottom_ndc(struct GKitViewport *viewport, struct GKitElement *element);
float gkit_calc_element_z_index_ndc(struct GKitViewport *viewport, struct GKitElement *element);

float gkit_calc_x_value_ndc(struct GKitViewport *viewport, unsigned int value);
float gkit_calc_y_value_ndc(struct GKitViewport *viewport, unsigned int value);

#endif /* GKIT_GL_MODEL_CALC_H */
