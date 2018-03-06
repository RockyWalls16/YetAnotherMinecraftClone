#version 330 core
in vec3 aPos;
in vec4 aColor;

out vec4 outColor;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
	outColor = aColor;
}