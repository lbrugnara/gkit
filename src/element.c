#include <stdio.h>
#include <glad/glad.h>
#include <fllib.h>
#include "element.h"
#include "internal/window.h"

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
    struct GKitElement *element = NULL;
    
    switch (type)
    {
        case GKIT_ELEMENT_RECT:
            element = fl_malloc(sizeof(struct GKitElementRect));
            break;

        case GKIT_ELEMENT_TEXT:
            element = fl_malloc(sizeof(struct GKitElementText));
            break;

        default:
            return NULL;
    }

    element->type = type;
    
    if (!gkit_internal_element_create(element))
    {
        fl_free(element);
        return NULL;
    }

    return element;
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
    gkit_internal_element_draw(element, viewport, type);

    if (element->type == GKIT_ELEMENT_RECT)
    {
        struct GKitElementRect *rect = (struct GKitElementRect*)element;
        if (rect->children)
        {
            struct FlListNode *node = fl_list_head(rect->children);
            while (node) {
                gkit_element_draw((GKitElement)node->value, viewport, type);
                node = node->next;
            }
        }
    }

    return true;
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
    gkit_internal_element_destroy(element);

    if (element->type == GKIT_ELEMENT_RECT)
    {
        struct GKitElementRect *rect = (struct GKitElementRect*)element;
        if (rect->children)
        {
            struct FlListNode *node = fl_list_head(rect->children);
            while (node) 
            {
                gkit_element_destroy((GKitElement)node->value);
                node = node->next;
            }
            fl_list_free(rect->children);
        }
    }
    else if (element->type == GKIT_ELEMENT_TEXT)
    {
        struct GKitElementText *text = (struct GKitElementText*)element;
        if (text->content)
            fl_cstring_free(text->content);
    }

    fl_free(element);
}
