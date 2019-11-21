#include <stdio.h>
#include <windows.h>
#include "core.h"
#include "window.h"
#include "font.h"
#include "element.h"

#include "internal/element.h"

int main(int argc, char **argv)
{
    if (!gkit_init(argc, argv))
    {
        fprintf(stderr, "Could not initialize the toolkit");
        gkit_exit();
        exit(-1);
    }

    GKitWindow window = gkit_window_create(800, 600, 0, 0, "gkit");

    if (!gkit_window_make_current(window))
    {
        fprintf(stderr, "Could not set current window");
        gkit_exit();
        exit(-1);
    }

    if (!gkit_font_load("resources/fonts/arial.ttf"))
    {
        fprintf(stderr, "Could not load the default font");
        gkit_exit();
        exit(-1);
    }
    
    GKitElement root = gkit_window_root(window);

    GKitElement element1 = gkit_element_create(GKIT_ELEMENT_RECT);
    element1->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .top = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .left = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .color = (struct GKitColor) {
            .red = 0,
            .green = 0,
            .blue = 210,
            .alpha = 1.0f,   
        },
        .zIndex = 0
    };
    gkit_element_add_child(root, element1);

    GKitElement element5 = gkit_element_create(GKIT_ELEMENT_RECT);
    element5->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 30
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 10
        },
        .top = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .left = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .color = (struct GKitColor) {
            .red = 25,
            .green = 100,
            .blue = 123,
            .alpha = 1.0f,   
        },
        .zIndex = GKIT_Z_INDEX_MAX
    };
    gkit_element_add_child(root, element5);


    GKitElement element2 = gkit_element_create(GKIT_ELEMENT_RECT);
    element2->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 400
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 200
        },
        .top = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .left = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 25
        },
        .color = (struct GKitColor) {
            .red = 0,
            .green = 210,
            .blue = 0,
            .alpha = 1.0f,   
        },
        .zIndex = 0
    };                                
    gkit_element_add_child(root, element2);    


    GKitElement element3 = gkit_element_create(GKIT_ELEMENT_RECT);
    element3->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .color = (struct GKitColor) {
            .red = 210,
            .green = 0,
            .blue = 210,
            .alpha = 1.0f,   
        },
        .zIndex = 0
    };
    gkit_element_add_child(element2, element3);


    GKitElement element4 = gkit_element_create(GKIT_ELEMENT_RECT);
    element4->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 50
        },
        .color = (struct GKitColor) {
            .red = 210,
            .green = 210,
            .blue = 0,
            .alpha = 1.0f,   
        }
    };
    gkit_element_add_child(element3, element4);

    GKitElement text = gkit_element_create(GKIT_ELEMENT_TEXT);
    text->style = (struct GKitStyle) {
        .width = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 100
        },
        .height = (struct GKitUnit) {
            .unit = GKIT_UNIT_PERCENTAGE,
            .value.percentage = 100
        },
        .top = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 0
        },
        .left = (struct GKitUnit) {
            .unit = GKIT_UNIT_PIXEL,
            .value.pixels = 0
        },
        .color = (struct GKitColor) {
            .red = 255,
            .green = 255,
            .blue = 255,
            .alpha = 1.0f,   
        },
        .zIndex = GKIT_Z_INDEX_MAX
    };
    ((struct GKitElementText*)text)->content = "abcdefghijklmnopqrstuvwxyz";
    gkit_element_add_child(element5, text);


    while (gkit_window_alive(window))
    {
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            gkit_window_close(window);
        }

        gkit_window_render(window);

        gkit_window_wait_events(window);
    }

    // GKitWindow will destroy all the GKitElements children of the root element
    gkit_window_destroy(window);

    gkit_exit();
    return 0;
}
