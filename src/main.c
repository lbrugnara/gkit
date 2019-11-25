#include <stdio.h>
#include <windows.h>
#include "core.h"
#include "window.h"
#include "font.h"
#include "element.h"
#include "gkml/markup.h"
#include "internal/element.h"

int main(int argc, char **argv)
{
    if (!gkit_init(argc, argv))
    {
        fprintf(stderr, "Could not initialize the toolkit");
        gkit_exit();
        exit(-1);
    }
    
    GKitElement root = NULL;
    if (fl_io_file_exists("main.gkml"))
        root = gkml_load("main.gkml");

    GKitWindow window = gkit_window_create(800, 600, 0, 0, "gkit", root);

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
