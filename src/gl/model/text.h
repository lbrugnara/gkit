#ifndef GKIT_GL_MODEL_TEXT_H
#define GKIT_GL_MODEL_TEXT_H

#include "../../model/internal/shader.h"

struct GLElementText {
    unsigned int vao;
    unsigned int vbo;
    GKitShader shader;
    unsigned int sid;
};

#endif /* GKIT_GL_MODEL_TEXT_H */
