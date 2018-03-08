#ifndef CLIENT_RENDER_LIGHT_DIRECTIONALLIGHTLOCATIONARRAY_H_
#define CLIENT_RENDER_LIGHT_DIRECTIONALLIGHTLOCATIONARRAY_H_

class Shader;
#include <string>

class DirectionalLightLocationArray
{
public:
	Shader* shader;
	int* lightColor;
	int* lightDirection;

	DirectionalLightLocationArray(Shader* shader, int size);

	void init(std::string arrayName, std::string variableName);
};

#endif