#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../windowintl.h"
#include "../window.h"

void on_glfw_window_resize(GLFWwindow *window, int width, int height)
{
    GKitWindow gkwindow = (GKitWindow)glfwGetWindowUserPointer(window);

    gkwindow->width = width;
    gkwindow->height = height;
    
    glViewport(0, 0, gkwindow->width, gkwindow->height);

    gkit_window_render(gkwindow);
    
    if (!gkwindow->onResize)
        return;

    size_t length = fl_array_length(gkwindow->onResize);
    for (size_t i=0; i < length; i++) 
        gkwindow->onResize[i](gkwindow->width, gkwindow->height);
}

bool gkit_internal_window_create(GKitWindow gkwindow)
{
    gkwindow->raw = glfwCreateWindow(gkwindow->width, gkwindow->height, gkwindow->title, NULL, NULL);;

    glfwSetWindowUserPointer((GLFWwindow*)gkwindow->raw, gkwindow);

    if (gkwindow->raw == NULL)
    {
        fprintf(stderr, "Failed to create the window object");
        return false;
    }

    return true;
}

bool gkit_internal_window_make_current(GKitWindow gkwindow)
{
    glfwMakeContextCurrent((GLFWwindow*)gkwindow->raw);
    glfwSetFramebufferSizeCallback((GLFWwindow*)gkwindow->raw, on_glfw_window_resize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, gkwindow->width, gkwindow->height);
    
    return true;
}

bool gkit_internal_window_alive(GKitWindow gkwindow)
{
    return !glfwWindowShouldClose((GLFWwindow*)gkwindow->raw);
}

void gkit_internal_window_close(GKitWindow gkwindow)
{
    glfwSetWindowShouldClose((GLFWwindow*)gkwindow->raw, true);
}

void gkit_internal_window_swap_buffers(GKitWindow gkwindow)
{
    glfwSwapBuffers((GLFWwindow*)gkwindow->raw);
}

void gkit_internal_window_process_events(GKitWindow gkwindow)
{
    glfwPollEvents();
}

void gkit_internal_window_wait_events(GKitWindow gkwindow)
{
    glfwWaitEvents();
}

void gkit_internal_window_destroy(GKitWindow gkwindow)
{
    glfwDestroyWindow((GLFWwindow*)gkwindow->raw);
}
