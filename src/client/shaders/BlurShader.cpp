#include <client/shaders/BlurShader.h>
#include <util/Logger.h>
#include <glm/glm.hpp>
#include <util/GLHeader.h>

void BlurShader::onDraw(glm::mat4 & modelMatrix, glm::mat3 & normalMatrix)
{
}

void BlurShader::onResize(int width, int height)
{
	Shader::use();

	glUniform2f(pixelSizeLocation, 1.0F / (float)width, 1.0F / (float)height);
}

void BlurShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");

	bindUniformLocation("uPixelSize", &pixelSizeLocation);
}
