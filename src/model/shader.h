#ifndef GKIT_MODEL_SHADER_H
#define GKIT_MODEL_SHADER_H

/*
 * Type: GKitShader
 *  A pointer to a <struct GKitShader> object
 */
typedef struct GKitShader *GKitShader;


/*
 * Function: gkit_shader_create
 *  Creates a new shader object using the provided vertex and fragment shaders
 *
 * Parameters:
 *  const char *vs_source - Vertex shader source program
 *  const char *fs_source - Fragment shader source program
 *
 * Returns:
 *  GKitShader - The compiled shader object
 *
 * Notes:
 *  The object returned by this function must be freed using the
 *  <gkit_shader_destroy> function.
 */
GKitShader gkit_shader_create(const char *vs_source, const char *fs_source);

/*
 * Function: gkit_shader_create_default
 *  Creates a shader object using a default vertex and fragment shader program
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  GKitShader - The compiled shader object
 *
 * Notes:
 *  The object returned by this function must be freed using the
 *  <gkit_shader_destroy> function.
 * 
 */
GKitShader gkit_shader_create_default(void);

GKitShader gkit_shader_create_from_file(const char *vs_file, const char *fs_file);

/*
 * Function: gkit_shader_destroy
 *  Releases the memory used by a <GKitShader> object
 *
 * Parameters:
 *  shader - Object to be freed
 *
 * Returns:
 *  This function does not return a value
 */
void gkit_shader_destroy(GKitShader shader);

#endif /* GKIT_MODEL_SHADER_H */
