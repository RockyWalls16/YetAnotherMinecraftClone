#include "client/shaders/PostProcessShader.h"
#include <client/render/GameRenderer.h>
#include <client/render/SkyRenderer.h>
#include <util/GLHeader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void PostProcessShader::use()
{
	Shader::use();

	dirLightLocationArray.updateLights();

	Camera* camera = GameRenderer::getInstance().getGameCamera();
	glm::vec3 camPos = camera->getLocation();
	glUniform3f(uniformCameraPosLocation, camPos.x, camPos.y, camPos.z);
}

void PostProcessShader::stop()
{
	Shader::stop();
}

void PostProcessShader::onResize(int width, int height)
{
	Shader::use();

	glUniform2f(screenSizeUniformLocation, width, height);
	glUniform2f(pixelSizeUniformLocation, 1 / (float) width, 1 / (float) height);
}

void PostProcessShader::bindAttributesAndUniforms()
{
	Shader::use();

	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");

	bindUniformLocation("screenSize", &screenSizeUniformLocation);
	bindUniformLocation("pixelSize", &pixelSizeUniformLocation);


	dirLightLocationArray.init("uDirLightAmount", "uDirLights", "direction", "color");
	bindUniformLocation("uCameraPos", &uniformCameraPosLocation);

	// Set G Buffer textures
	int positionGBuffer;
	int normalGBuffer;
	int albedoGBuffer;
	int lightInfoGBuffer;
	int depthGBuffer;
	bindUniformLocation("gPosition", &positionGBuffer);
	bindUniformLocation("gNormal", &normalGBuffer);
	bindUniformLocation("gAlbedo", &albedoGBuffer);
	bindUniformLocation("gLightInfo", &lightInfoGBuffer);
	bindUniformLocation("gDepth", &depthGBuffer);

	glUniform1i(positionGBuffer, 0);
	glUniform1i(normalGBuffer, 1);
	glUniform1i(albedoGBuffer, 2);
	glUniform1i(lightInfoGBuffer, 3);
	glUniform1i(depthGBuffer, 4);
}