#ifndef CLIENT_RENDER_LIGHT_DIRECTIONALLIGHT_H_
#define CLIENT_RENDER_LIGHT_DIRECTIONALLIGHT_H_

#include <client/render/light/Light.h>
#include <glm/vec3.hpp>

class DirectionalLight : public Light
{
public:
	glm::vec3 lightDirection;

	DirectionalLight(Color color, float ambiantAmount, glm::vec3 direction);

	void setLightDirection(glm::vec3 direction);
};

#endif