#version 330 core
in vec3 aPos;

out vec3 outFragPos;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
	outFragPos = aPos.rgb;
}