/*
 * DeferredBlockShader.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <client/render/GameRenderer.h>
#include <util/GLHeader.h>
#include <util/Logger.h>
#include <client/shaders/DeferredBlockShader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void DeferredBlockShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aColor");
	bindAttribute(2, "aNormal");
	bindAttribute(3, "aAtlasPos");

	bindUniformLocation("uModel", &uniformModelLocation);
	bindUniformLocation("uViewMat", &uniformViewLocation);
	bindUniformLocation("uViewProj", &uniformViewProjLocation);
	bindUniformLocation("uAtlasCellSize", &uniformAtlasCellSizeLocation);
	bindUniformLocation("albedoMap", &uniformAlbedoTexture);
	bindUniformLocation("specularMap", &uniformSpecularTexture);

	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	glUniform1i(uniformAlbedoTexture, 0);
	glUniform1i(uniformSpecularTexture, 1);
}

void DeferredBlockShader::use()
{
	Shader::use();

	Camera& camera = GameRenderer::getInstance().getGameCamera();
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniformMatrix4fv(uniformViewProjLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewProjMatrix()));
}

void DeferredBlockShader::stop()
{
	Shader::stop();
}

void DeferredBlockShader::setAtlasCellSize(float cellW, float cellH)
{
	Shader::use();
	glUniform2f(uniformAtlasCellSizeLocation, cellW, cellH);
	this->cellW = cellW;
	this->cellH = cellH;
}

void DeferredBlockShader::onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix)
{
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
