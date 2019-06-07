#ifndef GKIT_WINDOWINTL_H
#define GKIT_WINDOWINTL_H

#include "element.h"

/* About: Window internals
 *  The types and functions defined in this header are intended for private use only. Libraries referencing these
 *  types or functions directly will probably get breaking changes on newer versions.
 */

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
    GKitElement root;
    int width;
    int height;
    int x;
    int y;
    bool alive;
};

#endif /* GKIT_WINDOWINTL_H */
