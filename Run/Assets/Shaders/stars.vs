#version 330 core
in vec3 aPos;
in vec2 aStarInfo;

out float outShininess;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
	gl_PointSize = aStarInfo.y;
	outShininess = aStarInfo.x;
}