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
	float uPos = mod(coords.x, 1);
	float vPos = mod(coords.y, 1);
	vec2 texCoords = vec2(uPos * uAtlasCellSize.x + outAtlasPos.x, vPos * uAtlasCellSize.y + outAtlasPos.y);
	
	// Specular
	//vec3 unitToCamera = normalize(toCamera);
	//vec3 reflectDir = reflect(-unitToLight, unitNormal);
	
	//vec4 specTex = texture(specularMap, texCoords);
    //float specAmount = pow(max(dot(unitToCamera, reflectDir), 0.0), specTex.r * 255.0);
	//vec3 specular = specTex.g * specAmount * vec3(1.0, 1.0, 1.0);  

	gPosition = outFragPos;
	gNormal = normalize(outNormal);
	gAlbedo = texture(albedoMap, texCoords);
	gLightInfo = texture(specularMap, texCoords).rgb;
} 