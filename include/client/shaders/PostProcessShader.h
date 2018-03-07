/*
* DeferredBlockShader.h
*
*  Created on: 31 août 2017
*      Author: Valentin
*/

#ifndef CLIENT_SHADERS_POSTPROCESSSHADER_H_
#define CLIENT_SHADERS_POSTPROCESSSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class PostProcessShader : public Shader
{
private:
	int screenSizeUniformLocation;
	int pixelSizeUniformLocation;
	int uniformSunDirLocation;
	int uniformCameraPosLocation;

public:
	PostProcessShader(std::string name) : Shader(name), screenSizeUniformLocation(0), pixelSizeUniformLocation(0), uniformSunDirLocation(0){}

	virtual void use() override;
	virtual void stop() override;
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3 & normalMatrix) {}
	void onResize(int width, int height);

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif /* CLIENT_SHADERS_POSTPROCESSSHADER_H_ */
