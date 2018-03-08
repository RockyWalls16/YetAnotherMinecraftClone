#version 330 core

out vec4 gl_Color;

in vec3 outFragPos;
in vec3 outColor;
in vec3 outNormal;
in vec3 outRawNormal;
in vec2 outAtlasPos;

in vec3 toCamera;

struct DirLight
{
    vec3 direction;
    vec3 color;
};
uniform DirLight uSunLight;

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
	float vPos = coords.y - floor(coords.y);
	vec2 texCoords = vec2(uPos * uAtlasCellSize.x + outAtlasPos.x, vPos * uAtlasCellSize.y + outAtlasPos.y);
	
	// Diffuse light
	vec3 unitNormal = normalize(outNormal);
	vec3 unitToLight = normalize(-uSunLight.direction);
	vec3 diffuse = max(dot(unitNormal, unitToLight), 0.2) * uSunLight.color;

	// Specular
	vec3 unitToCamera = normalize(toCamera);
	vec3 reflectDir = reflect(-unitToLight, unitNormal);
	
	vec3 specularTex = texture(specularMap, texCoords).rgb;
	float specAmount = pow(max(dot(unitToCamera, reflectDir), 0.0), specularTex.r * 255.0);
	vec3 specular = specularTex.g * specAmount * uSunLight.color;
	
	// Final color
	vec4 texColor = texture(albedoMap, texCoords);
    gl_Color = vec4(diffuse, 1.0) * texColor * vec4(outColor, 1.0) + vec4(specular, 0.0);
} 
