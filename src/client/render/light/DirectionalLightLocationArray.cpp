#include <client/render/light/DirectionalLightLocationArray.h>
#include <client/shaders/Shader.h>
#include <util/GLHeader.h>

DirectionalLightLocationArray::DirectionalLightLocationArray(Shader* shader, int size) : shader(shader)
{
	lightColor = new int[size] {0};
	lightDirection = new int[size] {0};
}

void DirectionalLightLocationArray::init(std::string arrayName, std::string variableName)
{
	lightColor[0] = glGetUniformLocation(shader->getProgramId(), "");
}
