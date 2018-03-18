/*
* DeferredBlockShader.h
*
*  Created on: 31 août 2017
*      Author: Valentin
*/

#ifndef CLIENT_SHADERS_POSTPROCESSSHADER_H_
#define CLIENT_SHADERS_POSTPROCESSSHADER_H_

#include <client/shaders/Shader.h>
#include <client/render/light/LightCache.h>
#include <client/render/light/DirectionalLightLocationArray.h>
#include <string>


class DeferredLightingShader : public Shader
{
private:
	int screenSizeUniformLocation;
	int pixelSizeUniformLocation;
	int uniformSunColorLocation;
	int uniformSunDirLocation;
	int uniformCameraPosLocation;
	int uniformViewMatrixLocation;
	DirectionalLightLocationArray dirLightLocationArray;

public:
	DeferredLightingShader(std::string name) : Shader(name), dirLightLocationArray(this, MAX_DIR_LIGHT){}

	virtual void use() override;
	virtual void stop() override;
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3 & normalMatrix) {}
	virtual void onResize(int width, int height) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif /* CLIENT_SHADERS_POSTPROCESSSHADER_H_ */
