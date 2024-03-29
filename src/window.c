#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include "model/internal/element.h"
#include "internal/window.h"

static inline void set_default_style(struct GKitStyle *style, int width, int height)
{
    style->color = (struct GKitColor) {
        .red = 0xCA,
        .green = 0xCA,
        .blue = 0xCA,
        .alpha = 1.0f
    };

    style->layout = (struct GKitLayout) {
        .type = GKIT_LAYOUT_NONE,

        .overflow = GKIT_OVERFLOW_VISIBLE,

        .width  = { .unit = GKIT_UNIT_PIXEL, .value.pixels = width },
        .height = { .unit = GKIT_UNIT_PIXEL, .value.pixels = height },

        .position = {
            .left   = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .right  = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .top    = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .bottom = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
        },

        .margin = {
            .left   = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .right  = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .top    = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
            .bottom = { .unit = GKIT_UNIT_PIXEL, .value.pixels = 0 },
        }
    };

    style->zIndex = GKIT_Z_INDEX_MIN;
}

/*
 * Function: gkit_window_create
 *  Allocates memory for a <GKitWindow> instance and calls the <gkit_internal_window_create> to 
 *  instantiate a new window. It creates a <GKitElement> to be the root element to be rendered 
 *  when the <gkit_window_render> function is called with the new <GKitWindow> instance as a argument.
 *
 * Parameters:
 *  width - Window's width.
 *  height - Window's height.
 *  x - Window's x-axis position.
 *  y - Window's y-axis position.
 *  title - Window's title.
 *
 * Returns:
 *  GKitWindow - The new instance that identifies the created window.
 *
 */
GKitWindow gkit_window_create(int width, int height, int x, int y, char* title, GKitElement root)
{
    GKitWindow gkwindow = fl_malloc(sizeof(struct GKitWindow));

    gkwindow->width = width;
    gkwindow->height = height;
    gkwindow->x = x;
    gkwindow->y = y;
    gkwindow->title = fl_cstring_dup_n(title, strlen(title));
    gkwindow->onResize = NULL;
    gkwindow->onRender = NULL;

    if (!gkit_internal_window_create(gkwindow))
    {
        fprintf(stderr, "Failed to create the window object");
        gkit_window_destroy(gkwindow);
        return NULL;
    }

    if (root)
    {
        gkwindow->root = root;
        return gkwindow;
    }

    gkwindow->root = gkit_element_create(GKIT_ELEMENT_RECT);

    set_default_style(&gkwindow->root->style, width, height);

    return gkwindow;
}

GKitElement gkit_window_root_get(GKitWindow gkwindow)
{
    return gkwindow->root;
}

void gkit_window_root_set(GKitWindow gkwindow, GKitElement root)
{
    GKitElement old = gkwindow->root;
    gkwindow->root = root;

    if (old)
        gkit_element_destroy(old);
}

bool gkit_window_make_current(GKitWindow gkwindow)
{
    return gkit_internal_window_make_current(gkwindow);
}

/*
 * Function: gkit_window_render
 *  Renders the root <GKitElement> and all its children. The width and height of the root element
 *  are always updated to be the exact pixels so that all elements can be rendered relative to it
 *  if applicable (percentage units)
 *
 * Parameters:
 *  gkwindow - <GKitWindow> owner (and target) of the <GKitElement> to be rendered.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_window_render(GKitWindow gkwindow)
{
    gkit_internal_window_render(gkwindow);

    // Render the rectangles first so the text's blending occurs with the background elements ready
    gkit_element_draw(gkwindow->root, (struct GKitViewport) { gkwindow->width, gkwindow->height }, GKIT_ELEMENT_RECT);
    gkit_element_draw(gkwindow->root, (struct GKitViewport) { gkwindow->width, gkwindow->height }, GKIT_ELEMENT_TEXT);

    gkit_window_swap_buffers(gkwindow);
}

void gkit_window_on_render(GKitWindow gkwindow, GKitRenderFunction renderFunc)
{
    size_t length = 0;
    if (!gkwindow->onRender)
    {
        gkwindow->onRender = fl_array_new(sizeof(GKitResizeFunction), 1);
    }
    else
    {
        length = fl_array_length(gkwindow->onRender);
        gkwindow->onRender = fl_array_resize(gkwindow->onRender, length + 1);
    }

    gkwindow->onRender[length] = renderFunc;
}

void gkit_window_on_resize(GKitWindow gkwindow, GKitResizeFunction resizeFunc)
{
    size_t length = 0;
    if (!gkwindow->onResize)
    {
        gkwindow->onResize = fl_array_new(sizeof(GKitResizeFunction), 1);
    }
    else
    {
        length = fl_array_length(gkwindow->onResize);
        gkwindow->onResize = fl_array_resize(gkwindow->onResize, length + 1);
    }

    gkwindow->onResize[length] = resizeFunc;
}

bool gkit_window_alive(GKitWindow gkwindow)
{
    return gkit_internal_window_alive(gkwindow);
}

void gkit_window_close(GKitWindow gkwindow)
{
    gkit_internal_window_close(gkwindow);
}

void gkit_window_swap_buffers(GKitWindow gkwindow)
{
    gkit_internal_window_swap_buffers(gkwindow);
}

void gkit_window_process_events(GKitWindow gkwindow)
{
    gkit_internal_window_process_events(gkwindow);
}

void gkit_window_wait_events(GKitWindow gkwindow)
{
    gkit_internal_window_wait_events(gkwindow);
}

void* gkit_window_raw(GKitWindow gkwindow)
{
    return gkwindow->raw;
}

void gkit_window_destroy(GKitWindow gkwindow)
{
    gkit_internal_window_destroy(gkwindow);
    fl_cstring_free(gkwindow->title);
    gkit_element_destroy(gkwindow->root);
    free(gkwindow->onResize);
    free(gkwindow->onRender);
    free(gkwindow);
}
