#include <client/render/light/DirectionalLight.h>

DirectionalLight::DirectionalLight(Color color, glm::vec3 direction) : Light(color), lightDirection(direction)
{
}

void DirectionalLight::setLightDirection(glm::vec3 direction)
{
	lightDirection = direction;
	dirty = true;
}
