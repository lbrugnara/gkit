#include "identifier.h"

/*
 * Function: free_identifier_node
 *  Releases the memory of the <struct GkmlIdentifierNode> object
 *
 * Parameters:
 *  identifier_node - Object to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_node_id_free(struct GkmlIdentifierNode *id)
{
    if (!id)
        return;

    fl_cstring_free(id->name);
    fl_free(id);
}
