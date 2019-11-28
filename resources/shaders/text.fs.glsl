#version 330 core

layout(origin_upper_left) in vec4 gl_FragCoord;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

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

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
