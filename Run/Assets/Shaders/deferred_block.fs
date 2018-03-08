#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gLightInfo;

in vec3 outFragPos;
in vec3 outColor;
in vec3 outNormal;
in vec3 outRawNormal;
in vec2 outAtlasPos;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

uniform vec2 uAtlasCellSize;

vec2 getCoordinateFromNormal()
{
	if(outRawNormal.y != 0)
	{
		return vec2(outFragPos.x, outFragPos.z);
	}
	else if(outRawNormal.x != 0)
	{
		return vec2(outFragPos.z, outFragPos.y);
	}
	else
	{
		return vec2(outFragPos.x, outFragPos.y);
	}
}

void main()
{
	// Get texture coords
	vec2 coords = getCoordinateFromNormal();
	float uPos = coords.x - floor(coords.x);
	float vPos =  coords.y - floor(coords.y);
	vec2 texCoords = vec2(uPos * uAtlasCellSize.x + outAtlasPos.x, vPos * uAtlasCellSize.y + outAtlasPos.y);

	vec4 texColor = vec4(texture(albedoMap, texCoords));

	gPosition = outFragPos;
	gNormal = outNormal;
	gAlbedo = texColor;

	if(texColor.a == 0)
	{
		discard;
	}

	gLightInfo = texture(specularMap, texCoords).rgb;
} 