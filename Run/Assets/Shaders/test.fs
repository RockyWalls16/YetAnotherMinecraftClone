#version 330 core
out vec4 gl_Color;

in vec3 outColor;
in vec2 outTex;

uniform float uTime;

uniform sampler2D texture0;

void main()
{
    float power = (sin(uTime) + 1.0) / 2.0;
    gl_Color = mix(texture2D(texture0, outTex), vec4(outColor.rgb, 1.0), power);
} 