#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fllib.h>
#include "../core.h"

bool gkit_internal_init(int argc, char **argv)
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

void gkit_internal_exit()
{
    glfwTerminate();
}
