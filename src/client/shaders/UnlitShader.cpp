#include <client/shaders/UnlitShader.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <client/render/GameRenderer.h>
#include <client/render/Camera.h>
#include <util/Logger.h>

void UnlitShader::onDraw(glm::mat4 & modelMatrix, glm::mat3 & normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void UnlitShader::use()
{
	Shader::use();

	Camera& camera = GameRenderer::getInstance().getGameCamera();

	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewProjMatrix()));
}

void UnlitShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aColor");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
}
