#version 330 core

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
    if (gl_FragCoord.x < left || gl_FragCoord.x > right || gl_FragCoord.y < bottom || gl_FragCoord.y > top)
        discard;
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
