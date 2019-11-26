#include <stdio.h>
#include <fllib.h>
#include "element.h"
#include "internal/rect.h"
#include "internal/text.h"

/*
 * Function: gkit_element_create
 *  Allocates memory for a <GLElement> that contains the reference to a <GKitElement>.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 * GKitElement - Instance to the newly created element
 *
 */
GKitElement gkit_element_create(enum GKitElementType type)
{
    switch (type)
    {
        case GKIT_ELEMENT_RECT:
            return (GKitElement)gkit_rect_create();

        case GKIT_ELEMENT_TEXT:
            return (GKitElement)gkit_text_create();

        default:
            return NULL;
    }
}


/*
 * Function: gkit_element_add_child
 *  Adds a <GKitElement> as a child of another one. This function allocates in the
 *  parent <GKitElement> the memory to be able to track children elements.
 *
 * Parameters:
 *  parent - Parent <GKitElement> that will keep track of *child*
 *  child - Child <GKitElement>
 *
 * Returns:
 *  bool - *true* if the parent object is a valid container and the child
 *          element can be added to it, otherwise *false*.
 */
bool gkit_element_add_child(GKitElement parent, GKitElement child)
{
    if (parent->type != GKIT_ELEMENT_RECT)
        return false;

    struct GKitElementRect *rect = (struct GKitElementRect*)parent;

    if (!rect->children)
        rect->children = fl_list_new();

    fl_list_append(rect->children, child);

    child->parent = parent;

    return true;
}

/*
 * Function: gkit_element_draw
 *  Draws the <GKitElement> and all its children. If *element* has not been
 *  initialized, this function will create a shader for the element. If the shader element
 *  cannot be created, this function will fail and return *false* to indicate the error.
 *
 * Parameters:
 *  element - <GKitElement> to be drawn.
 * viewport - Current viewport's size
 *
 * Returns:
 *  bool - *true* on success. Otherwise it returns *false*.
 */
bool gkit_element_draw(GKitElement element, struct GKitViewport viewport, enum GKitElementType type)
{
    switch (element->type)
    {
        case GKIT_ELEMENT_RECT:
            return gkit_rect_draw((GKitElementRect)element, viewport, type);

        case GKIT_ELEMENT_TEXT:
            return gkit_text_draw((GKitElementText)element, viewport, type);

        default:
            return false;
    }
}


/*
 * Function: gkit_element_destroy
 *  Frees all the resources allocated for the <GLElement>, including the <GKitElement>. This function also frees
 *  all the resources for the children <GKitElement>s, that way, freeing the root node of a <GKitWindow> will result
 *  in all its <GKitElement> being freed too.
 *
 * Parameters:
 *  element - <GKitElement> (and <GLElement>) to be freed.
 *
 * Returns:
 *  void - This function does not return a value.
 */
void gkit_element_destroy(GKitElement element)
{
    switch (element->type)
    {
        case GKIT_ELEMENT_RECT:
            gkit_rect_destroy((GKitElementRect)element);
            break;

        case GKIT_ELEMENT_TEXT:
            gkit_text_destroy((GKitElementText)element);
            break;

        default:
            return;
    }
}
