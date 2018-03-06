#version 330 core
out vec4 gl_Color;

in vec2 outTex;

uniform vec2 screenSize;
uniform vec2 pixelSize;

uniform vec3 uSunDir;
uniform vec3 uCameraPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gLightInfo;

const float ambiant = 0.1F;

void main()
{
    vec4 albedo = texture(gAlbedo, outTex);

	if(albedo.a == 0.0)
	{
		discard;
	}

	vec3 lightInfo = texture(gLightInfo, outTex).rgb;
	vec3 fragPos = texture(gPosition, outTex).rgb;
    vec3 normal = texture(gNormal, outTex).rgb;

	// Diffuse
	vec3 unitToLight = normalize(-uSunDir);
	vec3 diffuse = max(dot(normal, unitToLight), 0.2) * vec3(1.0, 1.0, 1.0);

	// Specular
	vec3 unitToCamera = normalize(uCameraPos - fragPos);
	vec3 reflectDir = reflect(-unitToLight, normal);
	
    float specAmount = pow(max(dot(unitToCamera, reflectDir), 0.0), lightInfo.r * 255.0);
	vec3 specular = lightInfo.g * specAmount * vec3(1.0, 1.0, 1.0);  

	//gl_Color = vec4(albedo.rgb * diffuse + specular, albedo.a);
	gl_Color = vec4(albedo.rgb, 1.0);
} 