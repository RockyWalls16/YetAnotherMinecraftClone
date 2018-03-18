#include <client/render/light/DirectionalLightLocationArray.h>
#include <client/render/light/LightCache.h>
#include <client/shaders/Shader.h>
#include <util/GLHeader.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <client/render/GameRenderer.h>
#include <util/Logger.h>

DirectionalLightLocationArray::DirectionalLightLocationArray(Shader* shader, int size) : shader(shader), size(size)
{
	lightColor = new int[size] {0};
	lightDirection = new int[size] {0};
	lightAmbiant = new int[size] {0};
}

void DirectionalLightLocationArray::init(std::string dirLightAmount, std::string arrayName, std::string directionVariableName, std::string colorVariableName, std::string ambiantVariableName)
{
	nbLightLocation = glGetUniformLocation(shader->getProgramId(), dirLightAmount.c_str());

	for (int i = 0; i < size; i++)
	{
		lightDirection[i] = glGetUniformLocation(shader->getProgramId(), (arrayName + "[" + std::to_string(i) + "]." + directionVariableName).c_str());
		lightColor[i] = glGetUniformLocation(shader->getProgramId(), (arrayName + "[" + std::to_string(i) + "]." + colorVariableName).c_str());
		lightAmbiant[i] = glGetUniformLocation(shader->getProgramId(), (arrayName + "[" + std::to_string(i) + "]." + ambiantVariableName).c_str());
	}
}

void DirectionalLightLocationArray::updateLights(bool viewSpace)
{
	Camera& camera = GameRenderer::getInstance().getGameCamera();

	std::vector<DirectionalLight*>& lights = LightCache::getDirectionalLights();
	int length = std::min((int)lights.size(), size);
	for (int i = 0; i < length; i++)
	{
		glm::vec4 lDir = glm::vec4(lights[i]->lightDirection, 1.0F);
		
		if (viewSpace)
		{
			lDir = camera.getViewMatrix() * lDir;
			glUniform3f(lightDirection[i], lDir.x, lDir.y, lDir.z);
		}

		if (lights[i]->dirty)
		{
			if (!viewSpace)
			{
				glUniform3f(lightDirection[i], lDir.x, lDir.y, lDir.z);
			}
			glUniform3f(lightColor[i], lights[i]->lightColor.r, lights[i]->lightColor.g, lights[i]->lightColor.b);
			glUniform1f(lightAmbiant[i], lights[i]->ambiantAmount);
		}
	}
	glUniform1i(nbLightLocation, length);
}
