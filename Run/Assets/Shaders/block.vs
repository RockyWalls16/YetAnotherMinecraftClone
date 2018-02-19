#version 330 core
in vec3 aPos;
in vec3 aColor;
in vec3 aNormal;
in vec2 aAtlasPos;

out vec3 outFragPos;
out vec3 outColor;
out vec3 outNormal;
out vec3 outRawNormal;
out vec2 outAtlasPos;

uniform float uTime;
uniform mat4 uModel;
uniform mat4 uViewMat;
uniform mat4 uViewProj;
uniform mat3 uNormalMat;
uniform vec3 uSunPos;

void main()
{
	// Set vertex pos
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
	outFragPos = vec3(uModel * vec4(aPos, 1.0));

	outNormal = uNormalMat * aNormal;
	outRawNormal = aNormal;
    outColor = aColor;
	outAtlasPos = aAtlasPos;
}