#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fllib.h>
#include "core.h"

/*
 * Function: gkit_internal_init
 *  Each specific implementation will use this function to initialize all the resource
 *  that are necessary in order to setup the framework
 *
 * Parameters:
 *  argc - Command line arguments count.
 *  argv - Command line arguments.
 *
 * Returns:
 *  bool - *true* on success. Otherwise *false*.
 *
 * See Also:
 *  <gkit_internal_exit>
 *  <gkit_init>
 */
extern bool gkit_internal_init(int argc, char **argv);

/*
 * Function: gkit_internal_exit
 *  Each specific implementation will use this function to cleanup all the
 *  resources initialized with <gkit_internal_init>.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 * bool - *true* on success. Otherwise *false*.
 *
 * * See Also:
 *  <gkit_internal_init>
 *  <gkit_exit>
 */
extern bool gkit_internal_exit(void);

/*
 * Variable: initialized
 *  Flag to track if GKit has been initialized
 *
 */
static bool initialized = false;


/*
 * Variable: exit_functions
 *  List of functions to be invoked when <gkit_exit> is called.
 *
 * See Also:
 *  <gkit_exit>
 *  <GKitExitFunction>
 */
static FlList exit_functions = NULL;


/*
 * Function: gkit_init
 *  Flags GKit as initialized, and calls the <gkit_internal_init> function
 *  to initialize the specific implementation.
 *
 * Parameters:
 *  argc - Command line arguments count.
 *  argv - Command line arguments.
 *
 * Returns:
 *  bool - *true* on success. Otherwise *false*.
 *
 * See Also:
 *  <gkit_internal_init>
 */
bool gkit_init(int argc, char **argv)
{
    if (initialized)
        return false;

    initialized = true;

    exit_functions = fl_list_new();
    return gkit_internal_init(argc, argv);
}

void gkit_at_exit(GKitExitFunction func)
{
    fl_list_append(exit_functions, (const void*)func);
}


/*
 * Function: gkit_exit
 *  Calls all the functions registered with <gkit_at_exit> and after that calls
 *  <gkit_internal_exit> to make the cleanup of all the resources. After that clears
 *  the <initialized> flag to reset the GKit state.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_exit(void)
{
    if (!initialized)
        return;

    if (exit_functions)
    {
        struct FlListNode *node = fl_list_head(exit_functions);
        while (node) {
            ((GKitExitFunction)node->value)();
            node = node->next;
        }
        fl_list_delete(exit_functions);
    }

    gkit_internal_exit();

    initialized = false;
}
