#ifndef CLIENT_SHADERS_UNLITSHADER_H_
#define CLIENT_SHADERS_UNLITSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class UnlitShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;

public:
	UnlitShader(std::string name) : Shader(name), uniformModelLocation(0), uniformViewProjLocation(0){}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void use() override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif