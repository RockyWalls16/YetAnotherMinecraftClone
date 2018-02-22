/*
 * StaticShader.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <client/render/GameRenderer.h>
#include <util/GLHeader.h>
#include <util/Logger.h>
#include <client/shaders/StaticShader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void StaticShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aColor");
	bindAttribute(2, "aNormal");
	bindAttribute(3, "aAtlasPos");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
	bindUniformLocation("uNormalMat", &uniformNormalLocation);
	bindUniformLocation("uCameraPos", &uniformCameraPos);
	bindUniformLocation("uSunDir", &uniformSunDir);
	bindUniformLocation("uAtlasCellSize", &uniformAtlasCellSizeLocation);
	bindUniformLocation("albedo", &uniformAlbedoTexture);
	bindUniformLocation("specularMap", &uniformSpecularTexture);

	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	glUniform1i(uniformAlbedoTexture, 0);
	glUniform1i(uniformSpecularTexture, 1);
}

void StaticShader::use()
{
	Shader::use();

	Camera* camera = GameRenderer::getInstance().getGameCamera();
	
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewProjMatrix()));
	
	glm::vec3 camPos = camera->getLocation();
	glUniform3f(uniformCameraPos, camPos.x, camPos.y, camPos.z);

	glm::vec3 sunDir = GameRenderer::getInstance().getWorldRenderer()->getSunDirection();
	glUniform3f(uniformSunDir, sunDir.x, sunDir.y, sunDir.z);
}

void StaticShader::stop()
{
	Shader::stop();
}

void StaticShader::setAtlasCellSize(float cellW, float cellH)
{
	Shader::use();
	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	this->cellW = cellW;
	this->cellH = cellH;
}

void StaticShader::onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(uniformNormalLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}
