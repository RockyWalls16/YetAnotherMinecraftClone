#include <client/shaders/SSAOShader.h>
#include <client/textures/TextureLoader.h>
#include <math/MathUtil.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <client/render/GameRenderer.h>

#define SAMPLE_AMOUNT 16

void SSAOShader::onDraw(glm::mat4 & modelMatrix, glm::mat3 & normalMatrix)
{
}

void SSAOShader::onResize(int width, int height)
{
	Shader::use();

	glUniformMatrix4fv(uniformProjectionLocation, 1, FALSE, glm::value_ptr(GameRenderer::getInstance().getGameCamera().getProjectionMatrix()));
	glUniform2f(uniformNoiseScaleLocation, width / 4.0F, height / 4.0F);
}

Texture * SSAOShader::getNoiseTexture()
{
	return noiseTexture;
}

void SSAOShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");

	// Generate SSAO kernel
	for (unsigned int i = 0; i < SAMPLE_AMOUNT; ++i)
	{
		glm::vec3 sample(MathUtil::randF() * 2.0 - 1.0, MathUtil::randF() * 2.0 - 1.0, MathUtil::randF());
		sample = glm::normalize(sample);
		sample *= MathUtil::randF();
		float scale = (float)i / (float) SAMPLE_AMOUNT;
		scale = MathUtil::lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		glUniform3f(glGetUniformLocation(programId, ("samples[" + std::to_string(i) + "]").c_str()), sample.x, sample.y, sample.z);
	}

	// Generate Noise texture
	float noiseVec[16 * 3];
	for (unsigned int i = 0; i < 16 * 3; i += 3)
	{
		noiseVec[i] = MathUtil::randF() * 2.0F - 1.0F;
		noiseVec[i + 1] = MathUtil::randF() * 2.0F - 1.0F;
		noiseVec[i + 2] = 0.0F;
	}
	noiseTexture = TextureLoader::createTexture(noiseVec, 4, 4, GL_RGB16F, GL_RGB, false, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_FLOAT);

	// Buffer textures
	int positionGBuffer;
	int normalGBuffer;
	int depthGBuffer;
	int noiseTextureLocation;
	bindUniformLocation("gPosition", &positionGBuffer);
	bindUniformLocation("gNormal", &normalGBuffer);
	bindUniformLocation("noiseTexture", &noiseTextureLocation);
	glUniform1i(positionGBuffer, 0);
	glUniform1i(normalGBuffer, 1);
	glUniform1i(noiseTextureLocation, 2);

	// Bind projection matrix
	bindUniformLocation("uProjection", &uniformProjectionLocation);
	bindUniformLocation("uNoiseScale", &uniformNoiseScaleLocation);
}
