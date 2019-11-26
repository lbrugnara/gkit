#ifndef GKIT_CORE_INTERNAL_H
#define GKIT_CORE_INTERNAL_H

#include <stdbool.h>
#include "../core.h"

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
bool gkit_internal_init(int argc, char **argv);

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
bool gkit_internal_exit(void);

#endif /* GKIT_CORE_INTERNAL_H */
