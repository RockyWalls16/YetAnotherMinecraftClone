#include <client/shaders/Shader2D.h>
#include <util/GLHeader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <client/render/GameRenderer.h>

void Shader2D::onDraw(glm::mat4 & modelMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(uniformOrthoLocation, 1, GL_FALSE, glm::value_ptr(GameRenderer::getInstance().getOrthoMatrix()));
}

void Shader2D::bindAttributesAndUniforms()
{
	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uProjection", &uniformOrthoLocation);
}
