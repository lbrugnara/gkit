#ifndef GKIT_SHADER_H
#define GKIT_SHADER_H

/*
 * Type: GKitShader
 *  A pointer to a <struct GKitShader> object
 */
typedef struct GKitShader *GKitShader;

GKitShader gkit_shader_create(const char *vs_source, const char *fs_source);
GKitShader gkit_shader_create_default(void);
void gkit_shader_destroy(GKitShader shader);

#endif /* GKIT_SHADER_H */
