#version 330 core
out vec4 gl_Color;

in vec2 outTex;

uniform sampler2D screenTexture;

void main()
{
   gl_Color = vec4(texture(screenTexture, outTex).rgb, 1.0);
} 