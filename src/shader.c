#include <fllib.h>
#include "shader.h"
#include "internal/shader.h"

GKitShader gkit_shader_create(const char *vs_source, const char *fs_source)
{
    struct GKitShader *shader = fl_malloc(sizeof(struct GKitShader));

    if (!gkit_internal_shader_create(shader, vs_source, fs_source))
    {
        fl_free(shader);
        return NULL;
    }

    return shader;
}

GKitShader gkit_shader_create_default(void)
{
    struct GKitShader *shader = fl_malloc(sizeof(struct GKitShader));

    if (!gkit_internal_shader_create_default(shader))
    {
        fl_free(shader);
        return NULL;
    }

    return shader;
}

void gkit_shader_destroy(GKitShader shader)
{
    gkit_internal_shader_destroy(shader);
    fl_free(shader);
}
