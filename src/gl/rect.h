#ifndef GKIT_GL_RECT_H
#define GKIT_GL_RECT_H

#include <fllib.h>
#include "../internal/shader.h"
#include "../internal/element.h"
#include "../internal/window.h"

struct GLElementRect {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    struct GKitShader *shader;
    unsigned int sid;
    float vertices[12];
    int indices[6];
};

bool gkit_gl_rect_create(struct GKitElement *element);
bool gkit_gl_rect_draw(struct GKitElement *element, struct GKitViewport viewport);
void gkit_gl_rect_destroy(struct GKitElement *element);

#endif /* GKIT_GL_RECT_H */
