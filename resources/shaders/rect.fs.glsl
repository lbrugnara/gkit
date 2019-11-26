#version 330 core

out vec4 FragColor;
uniform vec4 color; 

uniform int left;
uniform int top;
uniform int right;
uniform int bottom;

void main()
{
    // We honor the origin of coordinates starting at the top-left of the viewport, so that we "invert" the
    // check for bottom and top
    if (gl_FragCoord.x < left || gl_FragCoord.x > right || gl_FragCoord.y > bottom || gl_FragCoord.y < top)
        discard;

    FragColor = color;
}