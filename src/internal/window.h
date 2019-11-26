#ifndef GKIT_WINDOW_INTERNAL_H
#define GKIT_WINDOW_INTERNAL_H

#include <stdbool.h>
#include "../window.h"

/*
 * Type: struct GKitWindow
 *  
 *  Represents a GKit window instance
 *
 */
struct GKitWindow {
    void *raw;
    void (**onResize)(int, int);
    void (**onRender)(void);
    char* title;
    struct GKitElement *root;
    int width;
    int height;
    int x;
    int y;
    bool alive;
};


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
bool gkit_internal_window_create(struct GKitWindow *window);

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
bool gkit_internal_window_make_current(struct GKitWindow *window);

/*
 * Function: gkit_internal_window_render
 *  The specific implementation that prepares for rendering a frame within
 *  the window.
 *
 * Parameters:
 *  <GKitWindow> window - Instance of the window to render
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
void gkit_internal_window_render(struct GKitWindow *window);

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
bool gkit_internal_window_alive(struct GKitWindow *window);

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
void gkit_internal_window_close(struct GKitWindow *window);

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
void gkit_internal_window_swap_buffers(struct GKitWindow *window);

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
void gkit_internal_window_process_events(struct GKitWindow *window);

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
void gkit_internal_window_wait_events(struct GKitWindow *window);

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
void gkit_internal_window_destroy(struct GKitWindow *window);


#endif /* GKIT_WINDOW_INTERNAL_H */
