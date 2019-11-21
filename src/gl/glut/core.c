#include <glad/glad.h>
#include <GL/freeglut.h>
#include <fllib.h>
#include "../../core.h"

bool gkit_internal_init(int argc, char **argv)
{
    if(gladLoadGL()) {
        // you need an OpenGL context before loading glad
        printf("I did load GL with no context!\n");
        exit(-1);
    }

    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

    return true;
}

void gkit_internal_exit()
{
}
