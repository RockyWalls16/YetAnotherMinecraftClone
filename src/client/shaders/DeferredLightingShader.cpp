#include "client/shaders/DeferredLightingShader.h"
#include <client/render/GameRenderer.h>
#include <client/render/SkyRenderer.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void DeferredLightingShader::use()
{
	Shader::use();

	dirLightLocationArray.updateLights(true);

	Camera& camera = GameRenderer::getInstance().getGameCamera();
	glm::vec3 camPos = camera.getViewMatrix() * glm::vec4(camera.getLocation(), 1.0F);
	glUniform3f(uniformCameraPosLocation, camPos.x, camPos.y, camPos.z);

	glUniformMatrix4fv(uniformViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
}

void DeferredLightingShader::stop()
{
	Shader::stop();
}

void DeferredLightingShader::onResize(int width, int height)
{
	Shader::use();

	glUniform2f(screenSizeUniformLocation, (float) width, (float) height);
	glUniform2f(pixelSizeUniformLocation, 1 / (float) width, 1 / (float) height);
}

void DeferredLightingShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");

	bindUniformLocation("screenSize", &screenSizeUniformLocation);
	bindUniformLocation("pixelSize", &pixelSizeUniformLocation);


	dirLightLocationArray.init("uDirLightAmount", "uDirLights", "direction", "color", "minAmbiant");
	bindUniformLocation("uCameraPos", &uniformCameraPosLocation);
	bindUniformLocation("uViewMatrix", &uniformViewMatrixLocation);
	
	// Set G Buffer textures
	int positionGBuffer;
	int normalGBuffer;
	int albedoGBuffer;
	int lightInfoGBuffer;
	int ssaoBuffer;
	int depthGBuffer;
	bindUniformLocation("gPosition", &positionGBuffer);
	bindUniformLocation("gNormal", &normalGBuffer);
	bindUniformLocation("gAlbedo", &albedoGBuffer);
	bindUniformLocation("gLightInfo", &lightInfoGBuffer);
	bindUniformLocation("ssaoTex", &ssaoBuffer);
	bindUniformLocation("gDepth", &depthGBuffer);

	glUniform1i(positionGBuffer, 0);
	glUniform1i(normalGBuffer, 1);
	glUniform1i(albedoGBuffer, 2);
	glUniform1i(lightInfoGBuffer, 3);
	glUniform1i(ssaoBuffer, 4);
	glUniform1i(depthGBuffer, 5);
}