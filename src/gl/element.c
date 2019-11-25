#include <stdio.h>
#include <glad/glad.h>
#include <fllib.h>

#include "rect.h"
#include "text.h"

#include "../shader.h"
#include "../internal/shader.h"
#include "../internal/element.h"
#include "../internal/window.h"

/*
 * Function: gkit_internal_element_create
 *  Allocates memory for a <GLElement> that contains the reference to a <GKitElement>.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 * GKitElement - Instance to the newly created element
 *
 */
bool gkit_internal_element_create(struct GKitElement *element)
{
    if (element == NULL || element->raw != NULL)
        return false;

    if (element->type == GKIT_ELEMENT_RECT)
    {
        return gkit_gl_rect_create(element);
    }
    else if (element->type == GKIT_ELEMENT_TEXT)
    {
        return gkit_gl_text_create(element);
    }

    return false;
}

/*
 * Function: gkit_internal_element_draw
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
bool gkit_internal_element_draw(struct GKitElement *element, struct GKitViewport viewport, enum GKitElementType type)
{
    switch (element->type)
    {
        case GKIT_ELEMENT_RECT:
            if (type == GKIT_ELEMENT_RECT)
                return gkit_gl_rect_draw(element, viewport);
            return true;

        case GKIT_ELEMENT_TEXT:
            if (type == GKIT_ELEMENT_TEXT)
                return gkit_gl_text_draw(element, viewport);
            return true;

        default:
            break;
    }
    
    return false;
}


/*
 * Function: gkit_internal_element_destroy
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
void gkit_internal_element_destroy(struct GKitElement *element)
{
    switch (element->type)
    {
        case GKIT_ELEMENT_RECT:
            gkit_gl_rect_destroy(element);
            break;

        case GKIT_ELEMENT_TEXT:
            gkit_gl_text_destroy(element);

        default:
            break;
    }
}
