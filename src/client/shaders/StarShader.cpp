#include <client/shaders/StarShader.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <client/render/GameRenderer.h>
#include <client/render/Camera.h>
#include <util/Logger.h>

void StarShader::onDraw(glm::mat4 & modelMatrix, glm::mat3 & normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void StarShader::use()
{
	Shader::use();

	Camera& camera = GameRenderer::getInstance().getGameCamera();
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewProjMatrix()));
}

void StarShader::setStarAlpha(float alpha)
{
	Shader::use();

	glUniform1f(uniformStarAlphaLocation, alpha);
}

void StarShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aStarInfo");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
	bindUniformLocation("uStarAlpha", &uniformStarAlphaLocation);
}
