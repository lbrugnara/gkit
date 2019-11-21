#include <fllib.h>
#include "../internal/shader.h"
#include "../internal/element.h"
#include "../internal/window.h"

struct GLElementText {
    unsigned int vao;
    unsigned int vbo;
    struct GKitShader *shader;
    unsigned int sid;
};

bool gkit_gl_text_create(struct GKitElement *element);
bool gkit_gl_text_draw(struct GKitElement *element, struct GKitViewport viewport);
void gkit_gl_text_destroy(struct GKitElement *element);
