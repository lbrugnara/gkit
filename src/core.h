#ifndef GKIT_CORE_H
#define GKIT_CORE_H

#include <stdbool.h>

/*
 * Type: GKitExitFunction
 *  
 *  Callback function expected by <gkit_at_exit> function
 *
 */
typedef void(*GKitExitFunction)(void);

/*
 * Function: gkit_init
 *  Initializes GKit. Needs to be called before calling any other GKit function
 *
 * Parameters:
 *  argc - Command line's argc.
 *  argv - Command line's argv.
 *
 * Returns:
 *  <bool> true on success. Otherwise returns false.
 *
 * See Also:
 * 
 *  <gkit_exit>
 */
bool gkit_init(int argc, char **argv);

/*
 * Function: gkit_at_exit
 *  Registers a callback to be invoked when the <gkit_exit> function is called
 *  to finish a GKit app.
 *
 * Parameters:
 *  func - Function to be called at GKit's exit
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkit_at_exit(GKitExitFunction func);

/*
 * Function: gkit_exit
 *  All the callbacks registered with the <gkit_at_exit> function will be invoked and all
 *  the resources allocated by GKit will be freed.
 *  After a call to the gkit_exit function, in order to be able to use GKit functions again, 
 *  it is required to call <gkit_init>.
 *
 * Returns:
 * 
 *  void
 * 
 * See Also:
 * 
 *  <gkit_init>
 *  <gkit_at_exit>
 * 
 */
void gkit_exit(void);

#endif /* GKIT_CORE_H */
