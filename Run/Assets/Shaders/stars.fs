#version 330 core

out vec4 gl_Color;

in float outShininess;

uniform float uStarAlpha;
uniform sampler2D starTexture;

void main()
{
    gl_Color = texture(starTexture, gl_PointCoord) * vec4(1.0, 1.0, 1.0, outShininess * uStarAlpha);
}