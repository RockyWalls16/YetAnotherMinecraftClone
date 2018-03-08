#include <client/shaders/SkySphereShader.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <client/render/GameRenderer.h>

void SkySphereShader::onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void SkySphereShader::use()
{
	Shader::use();

	Camera* camera = GameRenderer::getInstance().getGameCamera();
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewProjMatrix()));
}

void SkySphereShader::setColors(Color & topColor, Color & bottomColor)
{
	glUniform3f(uniformTopColorLocation, topColor.r, topColor.g, topColor.b);
	glUniform3f(uniformBottomColorLocation, bottomColor.r, bottomColor.g, bottomColor.b);
}

void SkySphereShader::bindAttributesAndUniforms()
{
	bindAttribute(0, "aPos");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);

	bindUniformLocation("topColor", &uniformTopColorLocation);
	bindUniformLocation("bottomColor", &uniformBottomColorLocation);
}
