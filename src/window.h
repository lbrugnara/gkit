#ifndef GKIT_WINDOW
#define GKIT_WINDOW

#include "element.h"

/*
 * Type: GKitWindow
 *  
 *  An opaque pointer to a window instance.
 *
 */
typedef struct GKitWindow *GKitWindow;

/*
 * Type: GKitResizeFunction
 *  
 *  Callback function expected by <gkit_window_on_resize>.
 *
 */
typedef void (*GKitResizeFunction)(int height, int width);

/*
 * Type: GKitRenderFunction
 *  
 *  Callback function expected by <gkit_window_on_render>.
 *
 */
typedef void (*GKitRenderFunction)(void);

/**
 * Function: gkit_window_create
 * 
 *  Creates a new window with the specified attributes.
 * 
 * Parameters:
 * 
 *  width - Window's width
 *  height - Window's height
 *  x - Window's x-axis position
 *  y - Window's y-axis position
 *  title - Window's title
 * 
 * Returns:
 *  <GKitWindow> - An instance of a GKitWindow
 * 
 */
GKitWindow gkit_window_create(int width, int height, int x, int y, char *title, GKitElement root);


/*
 * Function: gkit_window_root
 *  Returns a reference to a <GKitElement> that represents the root element of
 *  the window containing all the <GKitElement>s to be rendered.
 *
 * Parameters:
 *  gkwindow - Window to get the root <GKitElement> from.
 *
 * Returns:
 *  GKitElement - The root <GKitElement>.
 *
 */
GKitElement gkit_window_root(GKitWindow gkwindow);

/*
 * Function: gkit_window_make_current
 *  Set *gkwindow* as the current window in the thread where it is being called.
 *
 * Parameters:
 *  gkwindow - A GKitWindow instance.
 *
 * Returns:
 *  bool - *true* on success. Otherwise *false*.
 *
 */
bool gkit_window_make_current(GKitWindow gkwindow);

/*
 * Function: gkit_window_render
 *  Renders the root <GKitElement> and all its children.
 *
 * Parameters:
 *  gkwindow - <GKitWindow> to be rendered.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_window_render(GKitWindow gkwindow);

/*
 * Function: gkit_window_on_render
 *  Registers a <GKitRenderFunction> to be called when a window render event occur
 *
 * Parameters:
 *  gkwindow - <GKitWindow> where the render event happens
 *  renderFunc - <GKitRenderFunction> to be called when the render event occur
 *
 * Returns:
 * void - This function does not return a value.
 */
void gkit_window_on_render(GKitWindow gkwindow, GKitRenderFunction renderFunc);

/*
 * Function: gkit_window_on_resize
 *  Registers a <GKitResizeFunction> to be called when a window resize event occur
 *
 * Parameters:
 *  gkwindow - <GKitWindow> where the resize event happens
 *  resizeFunc - <GKitResizeFunction> to be called when the resize event occur
 *
 * Returns:
 * void - This function does not return a value.
 */
void gkit_window_on_resize(GKitWindow gkwindow, GKitResizeFunction resizeFunc);

/*
 * Function: gkit_window_alive
 *  Return true if the <GKitWindow> instance is still alive. If the user closes
 *  the window this function must return false.
 *
 * Parameters:
 *  gkwindow - <GKitWindow> instance to check if it is still alive
 *
 * Returns:
 *  bool - *true* if the <GKitWindow> is alive. *false* otherwise.
 *
 */
bool gkit_window_alive(GKitWindow gkwindow);

/*
 * Function: gkit_window_swap_buffers
 *  Swaps the buffers of the current <GKitWindow> if double buffered. 
 *
 * Parameters:
 *  gkwindow - Current <GKitWindow>.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_window_swap_buffers(GKitWindow gkwindow);

/*
 * Function: gkit_window_process_events
 *  Processes the pending events
 *
 * Parameters:
 *  gkwindow - <GKitWindow> instance
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_window_process_events(GKitWindow gkwindow);

/*
 * Function: gkit_window_wait_events
 *  Waits for new events to occur on the <GKitWindow> instance
 *
 * Parameters:
 *  gkwindow - <GKitWindow> instance
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_window_wait_events(GKitWindow gkwindow);

/*
 * Function: gkit_window_close
 *  Flags the <GKitWindow> instance for close.
 *
 * Parameters:
 * gkwindow - <GKitWindow> instance to be closed
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_window_close(GKitWindow gkwindow);


/*
 * Function: gkit_window_destroy
 *  Frees all the resources allocated by a <GKitWindow> instance
 *
 * Parameters:
 *  gkwindow - <GKitWindow> instance to be freed
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_window_destroy(GKitWindow gkwindow);


/*
 * Function: gkit_window_raw
 *  Returns the underlying handle of a <GKitWindow>. It is *implementation dependant*
 *  which means it is expected to break things if the underlying window handling library
 *  changes.
 *
 * Parameters:
 *  gkwindow - <GKitWindow> instance owner of the raw handle
 *
 * Returns:
 *  void* - Pointer to the raw handler
 *
 */
void* gkit_window_raw(GKitWindow gkwindow);

#endif /* GKIT_WINDOW */
