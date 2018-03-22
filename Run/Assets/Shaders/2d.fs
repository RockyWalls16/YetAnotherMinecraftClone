#version 330 core
out vec4 gl_Color;

in vec2 outTex;
in vec4 outColor;

uniform sampler2D texture0;

void main()
{
   gl_Color = texture(texture0, outTex) * outColor;
} 