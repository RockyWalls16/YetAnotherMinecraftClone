#include <client/shaders/SkyShader.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <client/render/GameRenderer.h>

void SkyShader::onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void SkyShader::use()
{
	Shader::use();

	Camera* camera = GameRenderer::getInstance().getGameCamera();
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewProjMatrix()));
}

void SkyShader::bindAttributesAndUniforms()
{
	bindAttribute(0, "aPos");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
}
