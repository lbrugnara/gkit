#include <glad/glad.h>
#include <fllib.h>
#include "shader.h"

const char *DEFAULT_VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n"
;

const char *DEFAULT_FRAGMENT_SHADER = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color; \n"
    "void main()\n"
    "{\n"
    "    FragColor = color;\n"
    "} \n"
;

unsigned int gkit_shader_new(void)
{
    // Compile the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &DEFAULT_VERTEX_SHADER, NULL);
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
    glShaderSource(fragmentShader, 1, &DEFAULT_FRAGMENT_SHADER, NULL);
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

    return shaderId;
}
