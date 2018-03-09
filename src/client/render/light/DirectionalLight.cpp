#include <client/render/light/DirectionalLight.h>

DirectionalLight::DirectionalLight(Color color, float ambiantAmount, glm::vec3 direction) : Light(color, ambiantAmount), lightDirection(direction)
{
}

void DirectionalLight::setLightDirection(glm::vec3 direction)
{
	lightDirection = direction;
	dirty = true;
}
