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
	int* lightAmbiant;
	int nbLightLocation;
	int size;

	DirectionalLightLocationArray(Shader* shader, int size);

	void init(std::string dirLightAmount, std::string arrayName, std::string directionVariableName, std::string colorVariableName, std::string ambiantVariableName);

	void updateLights(bool viewSpace);
};

#endif