#include <glad/glad.h>
#include <fllib.h>
#include "../internal/shader.h"

static const char *DEFAULT_VERTEX_SHADER =
    "#version 330 core"                                     "\n"
    "layout (location = 0) in vec3 aPos;"                   "\n"
    "void main()"                                           "\n"
    "{"                                                     "\n"
    "    gl_Position = vec4(aPos.xyz, 1.0);"                "\n"
    "}"                                                     "\n"
;

static const char *DEFAULT_FRAGMENT_SHADER = 
    "#version 330 core"             "\n"
    "out vec4 FragColor;"           "\n"
    "uniform vec4 color; "          "\n"
    "void main()"                   "\n"
    "{"                             "\n"
    "    FragColor = color;"        "\n"
    "}"                             "\n"
;

bool gkit_internal_shader_create(struct GKitShader *shader, const char *vs_source, const char *fs_source)
{
    // Compile the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs_source, NULL);
    glCompileShader(vertexShader);
    
    int  success;
    char vlog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vlog);
        fprintf(stdout, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", vlog);
        return false;
    }

    // Compile the fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs_source, NULL);
    glCompileShader(fragmentShader);

    char flog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, flog);
        fprintf(stdout, "ERROR::FRAGMENT::VERTEX::COMPILATION_FAILED\n%s\n", flog);
        return false;
    }

    // Create the shader program
    unsigned int shaderId = glCreateProgram();
    glAttachShader(shaderId, vertexShader);
    glAttachShader(shaderId, fragmentShader);
    glLinkProgram(shaderId);

    char plog[512];
    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderId, 512, NULL, plog);
        fprintf(stdout, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", plog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shader->raw = fl_malloc(sizeof(unsigned int));
    *(unsigned int*)shader->raw = shaderId;

    return true;
}

bool gkit_internal_shader_create_default(struct GKitShader *shader)
{
    return gkit_internal_shader_create(shader, DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
}

void gkit_internal_shader_destroy(struct GKitShader *shader)
{
    if (shader && shader->raw)
        fl_free(shader->raw);
}
