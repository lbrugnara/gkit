#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include <windows.h>
#include <glad/glad.h>
#include <GL/freeglut.h>
#include "../../core.h"
#include "../../window.h"
#include "../../internal/window.h"

static FlHashtable created_windows;

void clean_created_windows(void)
{
    fl_hashtable_free(created_windows);
}

void free_glut_id(struct GKitWindow *gkwin)
{
    if (!gkwin || !gkwin->raw)
        return;

    fl_free(gkwin->raw);
}

void on_glut_display(void)
{
    int glutId = glutGetWindow();

    struct GKitWindow *gkwin = (struct GKitWindow*)fl_hashtable_get(created_windows, &glutId);

    gkit_window_render(gkwin);

    if (!gkwin->onRender)
        return;

    size_t length = fl_array_length(gkwin->onRender);

    for (size_t i=0; i < length; i++)
        gkwin->onRender[i]();
}

void on_glut_reshape(int width, int height)
{
    int glutId = glutGetWindow();

    struct GKitWindow *gkwin = (struct GKitWindow*)fl_hashtable_get(created_windows, &glutId);

    gkwin->width = width;
    gkwin->height = height;
    
    glViewport(0, 0, gkwin->width, gkwin->height);
    
    if (!gkwin->onResize)
        return;

    size_t length = fl_array_length(gkwin->onResize);

    for (size_t i=0; i < length; i++) 
        gkwin->onResize[i](gkwin->width, gkwin->height);
}

bool gkit_internal_window_create(struct GKitWindow *gkwin)
{
    if (!created_windows)
    {
        created_windows = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_int,
            .key_comparer = fl_container_equals_int,
            .key_cleaner = fl_container_cleaner_pointer,            
            .key_allocator = fl_container_allocator_int,
            .value_cleaner = (FlContainerCleanupFunction)free_glut_id
        });

        gkit_at_exit(clean_created_windows);
    }

    glutInitWindowSize(gkwin->width, gkwin->height);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    int glutId = glutCreateWindow(gkwin->title);

    if (!glutId)
    {
        fprintf(stderr, "Failed to create the window object");
        return false;
    }

    gkwin->raw = fl_malloc(sizeof(int));
    *(int*)gkwin->raw = glutId;

    fl_hashtable_add(created_windows, gkwin->raw, gkwin);

    return true;
}

bool gkit_internal_window_make_current(struct GKitWindow *gkwin)
{
    glutSetWindow(*(int*)gkwin->raw);
    glutDisplayFunc(on_glut_display);
    glutReshapeFunc(on_glut_reshape);

    if (!gladLoadGL())
    {
        fprintf(stderr, "Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, gkwin->width, gkwin->height);

    gkwin->alive = true;
    
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

bool gkit_internal_window_alive(struct GKitWindow *gkwin)
{
    int glutId = glutGetWindow();
    return glutId && glutId == *(int*)gkwin->raw && gkwin->alive;
}

void gkit_internal_window_close(struct GKitWindow *gkwin)
{
    glutLeaveMainLoop();
    gkwin->alive = false;
}

void gkit_internal_window_swap_buffers(struct GKitWindow *gkwin)
{
    int glutId = glutGetWindow();

    if (glutId == 0)
        return;

    glutSwapBuffers();
}

void gkit_internal_window_process_events(struct GKitWindow *gkwin)
{
    if (!gkwin->alive)
        return;

    glutMainLoopEvent();
}

void gkit_internal_window_wait_events(struct GKitWindow *gkwin)
{
    if (!gkwin->alive)
        return;

    MSG msg = { 0 };
    if (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void gkit_internal_window_destroy(struct GKitWindow *gkwin)
{
    glutDestroyWindow(*(int*)gkwin->raw);
    fl_hashtable_remove(created_windows, gkwin->raw, true);
}
