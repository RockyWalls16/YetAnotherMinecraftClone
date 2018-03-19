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

out vec3 toCamera;

uniform mat4 uModel;
uniform mat4 uViewMat;
uniform mat4 uViewProj;

uniform vec3 uCameraPos;

void main()
{
	// Set vertex pos
	vec4 worldPosition = uModel * vec4(aPos, 1.0);
    gl_Position = uViewProj * worldPosition;
	outFragPos = vec3(uModel * vec4(aPos, 1.0));

	outNormal = normalize(aNormal);
	outRawNormal = aNormal;
    outColor = aColor;
	outAtlasPos = aAtlasPos;

	toCamera = uCameraPos - worldPosition.xyz;
}
