#version 330 core
out vec4 gl_Color;

in vec3 outFragPos;

uniform vec3 topColor;
uniform vec3 bottomColor;

void main()
{
	float h = normalize(outFragPos).y;
    gl_FragColor = vec4(mix(bottomColor, topColor, max(pow(max(h, 0.0), 0.8), 0.0)), 1.0);
} 