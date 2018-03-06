#version 330 core

//layout (location = 0) out vec3 gPosition;
//layout (location = 1) out vec3 gNormal;
//layout (location = 2) out vec4 gAlbedo;
//layout (location = 3) out vec3 gLightInfo;
out vec4 glColor;

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

float near = 0.1; 
float far  = 1000.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	// Get texture coords
	vec2 coords = getCoordinateFromNormal();
	float uPos = coords.x - floor(coords.x);
	float vPos =  coords.y - floor(coords.y);
	vec2 texCoords = vec2(uPos * uAtlasCellSize.x + outAtlasPos.x, vPos * uAtlasCellSize.y + outAtlasPos.y);

	// Specular
	//vec3 unitToCamera = normalize(toCamera);
	//vec3 reflectDir = reflect(-unitToLight, unitNormal);
	
	//vec4 specTex = texture(specularMap, texCoords);
    //float specAmount = pow(max(dot(unitToCamera, reflectDir), 0.0), specTex.r * 255.0);
	//vec3 specular = specTex.g * specAmount * vec3(1.0, 1.0, 1.0);  

	//gPosition = outFragPos;
	//gNormal = outNormal;
	//gAlbedo = texture(albedoMap, texCoords);
	//float depth = LinearizeDepth(gl_FragCoord.z) / far;
	//gAlbedo = vec4(depth, depth, depth, 1.0);
	//gLightInfo = texture(specularMap, texCoords).rgb;

	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	glColor = vec4(depth, depth, depth, 1.0);
	//glColor = texture(albedoMap, texCoords);
} 