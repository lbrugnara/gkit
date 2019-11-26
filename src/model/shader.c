#include <fllib.h>
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

GKitShader gkit_shader_create_from_file(const char *vs_file, const char *fs_file)
{
    struct GKitShader *shader = fl_malloc(sizeof(struct GKitShader));

    const char *vertex_shader = fl_io_file_read_all_text(vs_file);
    const char *fragment_shader = fl_io_file_read_all_text(fs_file);

    if (!gkit_internal_shader_create(shader, vertex_shader, fragment_shader))
    {
        fl_free(shader);
        fl_cstring_free(vertex_shader);
        fl_cstring_free(fragment_shader);
        return NULL;
    }

    fl_cstring_free(vertex_shader);
    fl_cstring_free(fragment_shader);

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
