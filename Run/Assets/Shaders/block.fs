#version 330 core

out vec4 gl_Color;

in vec3 outFragPos;
in vec3 outColor;
in vec3 outNormal;
in vec3 outRawNormal;
in vec2 outAtlasPos;

uniform sampler2D texture0;
uniform vec3 uSunPos;
uniform vec3 uCameraPos;
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
	vec2 coords = getCoordinateFromNormal();
	float uPos = mod(coords.x, 1);
	float vPos = mod(coords.y, 1);
	vec2 texCoords = vec2(uPos * uAtlasCellSize.x + outAtlasPos.x, vPos * uAtlasCellSize.y + outAtlasPos.y);

	vec3 normal = normalize(outNormal);
	vec3 lightDir = normalize(uSunPos - outFragPos);
	// Diffuse light
	float diffuse = max(dot(normal, lightDir), 0.0);

	// Specular
	vec3 viewDir = normalize(uCameraPos - outFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);  
	
	vec4 texColor = texture2D(texture0, texCoords);
    gl_Color = vec4(texColor.rgb * outColor.rgb * (diffuse * (1 - 0.4F) + 0.4F), texColor.a);
} 