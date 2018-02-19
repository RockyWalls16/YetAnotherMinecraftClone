#version 330 core
out vec4 gl_Color;

in vec3 outFragPos;

void main()
{
	vec3 top = vec3(0.5, 0.65, 1.0);
	vec3 bottom = vec3(0.71, 0.81, 1.0);
	
	float h = normalize(outFragPos).y;
    gl_FragColor = vec4(mix(bottom, top, max(pow(max(h, 0.0), 0.8), 0.0)), 1.0);
} 