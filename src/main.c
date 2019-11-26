#include <stdio.h>
#include <windows.h>
#include <fllib.h>
#include "core.h"
#include "window.h"
#include "model/font.h"
#include "model/element.h"
#include "gkml/gkml.h"

static long ms_per_frame = 1000 / 60; // We want approximately 60 frames per second (1000 miliseconds)

int main(int argc, char **argv)
{
    if (!gkit_init(argc, argv))
    {
        fprintf(stderr, "Could not initialize the toolkit");
        gkit_exit();
        exit(-1);
    }

    GKitWindow window = gkit_window_create(800, 600, 0, 0, "gkit", NULL);

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

    FlTimer timer = fl_timer_create();

    unsigned long long last_update_timestamp = 0;
    while (gkit_window_alive(window))
    {
        // New tick starts here
        fl_timer_tick(timer);

        if (fl_io_file_exists("main.gkml"))
        {
            unsigned long long current_timestamp = 0;
            fl_io_file_get_modified_timestamp("main.gkml", &current_timestamp);

            if (current_timestamp > last_update_timestamp)
            {
                if (fl_io_file_exists("main.gkml"))
                {
                    GKitElement root = gkml_load("main.gkml");
                    if (root)
                        gkit_window_root_set(window, root);
                }
                gkit_window_render(window);

                last_update_timestamp = current_timestamp;
            }
        }

        gkit_window_process_events(window);

        // Current tick ends here
        long frame_elapsed_ms = fl_timer_tick(timer);
        if (frame_elapsed_ms < ms_per_frame)
            fl_system_sleep(ms_per_frame - frame_elapsed_ms);
    }

    fl_timer_free(timer);

    // GKitWindow will destroy all the GKitElements children of the root element
    gkit_window_destroy(window);

    gkit_exit();
    return 0;
}
