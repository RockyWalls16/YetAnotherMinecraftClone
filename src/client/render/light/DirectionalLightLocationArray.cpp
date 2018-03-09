#include <client/render/light/DirectionalLightLocationArray.h>
#include <client/render/light/LightCache.h>
#include <client/shaders/Shader.h>
#include <util/GLHeader.h>
#include <algorithm>
#include <util/Logger.h>

DirectionalLightLocationArray::DirectionalLightLocationArray(Shader* shader, int size) : shader(shader), size(size)
{
	lightColor = new int[size] {0};
	lightDirection = new int[size] {0};
}

void DirectionalLightLocationArray::init(std::string dirLightAmount, std::string arrayName, std::string directionVariableName, std::string colorVariableName)
{
	nbLightLocation = glGetUniformLocation(shader->getProgramId(), dirLightAmount.c_str());

	for (int i = 0; i < size; i++)
	{
		lightDirection[i] = glGetUniformLocation(shader->getProgramId(), (arrayName + "[" + std::to_string(i) + "]." + directionVariableName).c_str());
		lightColor[i] = glGetUniformLocation(shader->getProgramId(), (arrayName + "[" + std::to_string(i) + "]." + colorVariableName).c_str());
	}
}

void DirectionalLightLocationArray::updateLights()
{
	std::vector<DirectionalLight*>& lights = LightCache::getDirectionalLights();
	int length = std::min((int)lights.size(), size);
	for (int i = 0; i < length; i++)
	{
		if (lights[i]->dirty)
		{
			glUniform3f(lightDirection[i], lights[i]->lightDirection.x, lights[i]->lightDirection.y, lights[i]->lightDirection.z);
			glUniform3f(lightColor[i], lights[i]->lightColor.r, lights[i]->lightColor.g, lights[i]->lightColor.b);
			lights[i]->dirty = false;
		}
	}
	glUniform1i(nbLightLocation, length);
}
