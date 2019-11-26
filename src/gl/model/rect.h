#ifndef GKIT_GL_MODEL_RECT_H
#define GKIT_GL_MODEL_RECT_H

#include "../../model/internal/shader.h"

struct GLElementRect {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    GKitShader shader;
    unsigned int sid;
    float vertices[12];
    int indices[6];
};

#endif /* GKIT_GL_MODEL_RECT_H */
