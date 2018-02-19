#version 330 core
out vec4 gl_Color;

in vec2 outTex;

uniform sampler2D texture0;

void main()
{
   gl_Color = texture(texture0, outTex);
  // gl_Color = vec4(1.0, 0.0, 0.0, 1.0);
} 