#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include <glad/glad.h>
#include "windowintl.h"
#include "element.h"
#include "window.h"

/*
 * Function: gkit_internal_window_create
 *  The specific implementation hooks in to create the actual window and saves a reference to it
 *  in the *raw* member of a <GKitWindow>
 *
 * Parameters:
 *  window - <GKitWindow> instance owner of the *raw* member to be initialized
 *
 * Returns:
 *  bool - *true* on success. Otherwise *false*.
 *
 * See Also:
 *  <gkit_window_create>
 *  <gkit_internal_window_destroy>
 */
extern bool gkit_internal_window_create(GKitWindow window);

/*
 * Function: gkit_internal_window_make_current
 *  The specific implementation sets the <GKitWindow> as the current window in the calling thread
 *
 * Parameters:
 *  window - <GKitWindow> instance to set as the current window
 *
 * Returns:
 *  bool - *true* on success. Otherwise *false*.
 *
 * See Also:
 *  <gkit_window_make_current>
 */
extern bool gkit_internal_window_make_current(GKitWindow window);


/*
 * Function: gkit_internal_window_alive
 *  The specific implementation checks if the <GKitWindow> instance is still alive
 *
 * Parameters:
 *  window - <GKitWindow> instance to check if it is still alive
 *
 * Returns:
 *  bool - *true* if the <GKitWindow> is still alive. Otherwise *false*.
 *
 * See Also:
 *  <gkit_window_alive>
 */
extern bool gkit_internal_window_alive(GKitWindow window);

/*
 * Function: gkit_internal_window_close
 *  The specific implementation flags the <GKitWindow> to request a window close
 *
 * Parameters:
 *  window - <GKitWindow> to be closed
 *
 * Returns:
 *  void - This function does not return a value.
 * 
 * See Also:
 *  <gkit_window_close>
 */
extern void gkit_internal_window_close(GKitWindow window);

/*
 * Function: gkit_internal_window_swap_buffers
 *  The specific implementation triggers a swap buffer in the <GKitWindow> instance (if double buffering is enabled)
 *
 * Parameters:
 *  window - <GKitWindow> instance target of the double buffering
 *
 * Returns:
 *  void - This function does not return a value.
 * 
 * See Also:
 *  <gkit_window_swap_buffers>
 */
extern void gkit_internal_window_swap_buffers(GKitWindow window);

/*
 * Function: gkit_internal_window_process_events
 *  The specific implementation processes the pending events on the <GKitWindow> and returns immediately.
 *
 * Parameters:
 *  window - <GKitWindow> target of the pending events to be processed
 *
 * Returns:
 *  void - This function does not return a value.
 * 
 * See Also:
 *  <gkit_window_process_events>
 */
extern void gkit_internal_window_process_events(GKitWindow window);

/*
 * Function: gkit_internal_window_wait_events
 *  The specific implementation puts the calling thread to sleep, waiting for events on the <GKitWindow>.
 *
 * Parameters:
 *  window - <GKitWindow> that waits for incoming events.
 *
 * Returns:
 *  void - This function does not return a value.
 * 
 * See Also:
 *  <gkit_window_wait_events>
 */
extern void gkit_internal_window_wait_events(GKitWindow window);

/*
 * Function: gkit_internal_window_destroy
 *  The specific implementation cleanups the resources allocated with <gkit_internal_window_create>.
 *
 * Parameters:
 *  window - <GKitWindow> target window to be destroyed.
 *
 * Returns:
 *  void - This function does not return a value.
 * 
 * See Also:
 *  <gkit_window_destroy>
 *  <gkit_internal_window_create>
 */
extern void gkit_internal_window_destroy(GKitWindow window);


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
GKitWindow gkit_window_create(int width, int height, int x, int y, char* title)
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

    gkwindow->root = gkit_element_create();

    gkwindow->root->style.width = (struct GKitUnit){ .value.pixels = width, .unit = GKIT_UNIT_PIXEL };
    gkwindow->root->style.height = (struct GKitUnit){ .value.pixels = height, .unit = GKIT_UNIT_PIXEL };

    gkwindow->root->style.zIndex = GKIT_Z_INDEX_MIN;
    gkwindow->root->style.color.red = 255;
    gkwindow->root->style.color.green = 255;
    gkwindow->root->style.color.blue = 255;
    gkwindow->root->style.color.alpha = 1.0f;

    return gkwindow;
}

GKitElement gkit_window_root(GKitWindow gkwindow)
{
    return gkwindow->root;
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Root always honors viewport width and height
    gkwindow->root->style.width.value.pixels = gkwindow->width;
    gkwindow->root->style.width.unit = GKIT_UNIT_PIXEL;
    gkwindow->root->style.height.value.pixels = gkwindow->height;
    gkwindow->root->style.height.unit = GKIT_UNIT_PIXEL;

    gkit_element_draw(gkwindow->root, (struct GKitViewport) { gkwindow->width, gkwindow->height });

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
    fl_cstring_delete(gkwindow->title);
    gkit_element_destroy(gkwindow->root);
    free(gkwindow->onResize);
    free(gkwindow->onRender);
    free(gkwindow);
}
