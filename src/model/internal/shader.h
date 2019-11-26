#ifndef GKIT_INTERNAL_SHADER_H
#define GKIT_INTERNAL_SHADER_H

#include <stdbool.h>
#include "../shader.h"

struct GKitShader {
    void *raw;
};

bool gkit_internal_shader_create(GKitShader shader, const char *vs_source, const char *fs_source);
bool gkit_internal_shader_create_default(GKitShader shader);
void gkit_internal_shader_destroy(GKitShader shader);

#endif /* GKIT_INTERNAL_SHADER_H */
