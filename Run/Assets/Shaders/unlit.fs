#version 330 core

out vec4 gl_Color;

in vec4 outColor;

void main()
{
    gl_Color = outColor;
} 