/*
* StaticShader.h
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
public:
	PostProcessShader(std::string name) : Shader(name) {}

	virtual void use() override;
	virtual void stop() override;
	virtual void onDraw(glm::mat4& modelMatrix) {}

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif /* CLIENT_SHADERS_POSTPROCESSSHADER_H_ */
