#version 330 core
out float gl_Color;

in vec2 outTex;

const int samplesAmount = 16;
const float radius = 0.4;
const float bias = 0.045;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTexture;

uniform mat4 uProjection;
uniform vec3 samples[samplesAmount];
uniform vec2 uNoiseScale;

void main()
{
	// Read from buffer
	vec3 fragPos = texture(gPosition, outTex).xyz;
	vec3 normal = texture(gNormal, outTex).rgb;
	vec3 randomVec = texture(noiseTexture, outTex * uNoiseScale).xyz;

	// Calculate tangent
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for(int i = 0; i < samplesAmount; ++i)
	{
		vec3 sample = TBN * samples[i]; // Convert samples to View space
		sample = fragPos + sample * radius; 

		vec4 offset = vec4(sample, 1.0);
		offset = uProjection * offset;
		offset.xyz /= offset.w; // To ndc
		offset.xyz  = offset.xyz * 0.5 + 0.5; // To [0.0-1.0]

		float sampleDepth = texture(gPosition, offset.xy).z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
	}  

	occlusion = 1.0 - (occlusion / samplesAmount);
	gl_Color = pow(occlusion, 2);
} 