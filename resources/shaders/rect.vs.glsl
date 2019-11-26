#version 330 core

layout (location = 0) in vec3 rect_pos;

void main()
{
    gl_Position = vec4(rect_pos.xyz, 1.0);
}
