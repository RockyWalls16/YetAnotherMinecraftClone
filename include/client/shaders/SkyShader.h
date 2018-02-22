#ifndef CLIENT_SHADERS_SKYSHADER_H_
#define CLIENT_SHADERS_SKYSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class SkyShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;

public:
	SkyShader(std::string name) : Shader(name), uniformModelLocation(0), uniformViewProjLocation(0){}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void SkyShader::use() override;
private:
	virtual void bindAttributesAndUniforms() override;
};


#endif