#include <client/render/GameRenderer.h>
#include <client/render/SkyRenderer.h>
#include <util/GLHeader.h>
#include <util/Logger.h>
#include <client/shaders/ForwardBlockShader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void ForwardBlockShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aColor");
	bindAttribute(2, "aNormal");
	bindAttribute(3, "aAtlasPos");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
	bindUniformLocation("uNormalMat", &uniformNormalLocation);
	bindUniformLocation("uAtlasCellSize", &uniformAtlasCellSizeLocation);
	bindUniformLocation("albedoMap", &uniformAlbedoTexture);
	bindUniformLocation("specularMap", &uniformSpecularTexture);

	dirLightLocationArray.init("uDirLightAmount", "uDirLights", "direction", "color", "minAmbiant");

	bindUniformLocation("uCameraPos", &uniformCameraPosLocation);

	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	glUniform1i(uniformAlbedoTexture, 0);
	glUniform1i(uniformSpecularTexture, 1);
}

void ForwardBlockShader::use()
{
	Shader::use();

	Camera& camera = GameRenderer::getInstance().getGameCamera();
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewProjMatrix()));

	dirLightLocationArray.updateLights(false);

	glm::vec3 camPos = camera.getLocation();
	glUniform3f(uniformCameraPosLocation, camPos.x, camPos.y, camPos.z);
}

void ForwardBlockShader::stop()
{
	Shader::stop();
}

void ForwardBlockShader::setAtlasCellSize(float cellW, float cellH)
{
	Shader::use();
	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	this->cellW = cellW;
	this->cellH = cellH;
}

void ForwardBlockShader::onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(uniformNormalLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}
