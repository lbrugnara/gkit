#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../window.h"
#include "../../internal/window.h"

void on_glfw_window_resize(GLFWwindow *window, int width, int height)
{
    struct GKitWindow *gkwindow = (struct GKitWindow*)glfwGetWindowUserPointer(window);

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

bool gkit_internal_window_create(struct GKitWindow *gkwindow)
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

bool gkit_internal_window_make_current(struct GKitWindow *gkwindow)
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

void gkit_internal_window_render(struct GKitWindow *gkwindow)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool gkit_internal_window_alive(struct GKitWindow *gkwindow)
{
    return !glfwWindowShouldClose((GLFWwindow*)gkwindow->raw);
}

void gkit_internal_window_close(struct GKitWindow *gkwindow)
{
    glfwSetWindowShouldClose((GLFWwindow*)gkwindow->raw, true);
}

void gkit_internal_window_swap_buffers(struct GKitWindow *gkwindow)
{
    glfwSwapBuffers((GLFWwindow*)gkwindow->raw);
}

void gkit_internal_window_process_events(struct GKitWindow *gkwindow)
{
    glfwPollEvents();
}

void gkit_internal_window_wait_events(struct GKitWindow *gkwindow)
{
    glfwWaitEvents();
}

void gkit_internal_window_destroy(struct GKitWindow *gkwindow)
{
    glfwDestroyWindow((GLFWwindow*)gkwindow->raw);
}
