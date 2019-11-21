#ifndef GKIT_SHADER_INTERNAL_H
#define GKIT_SHADER_INTERNAL_H

#include <fllib.h>

struct GKitShader {
    void *raw;
};

bool gkit_internal_shader_create(struct GKitShader *shader, const char *vs_source, const char *fs_source);
bool gkit_internal_shader_create_default(struct GKitShader *shader);
void gkit_internal_shader_destroy(struct GKitShader *shader);

#endif /* GKIT_SHADER_INTERNAL_H */
