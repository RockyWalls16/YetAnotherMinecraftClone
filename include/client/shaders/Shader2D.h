#ifndef CLIENT_SHADERS_SHADER2D_H_
#define CLIENT_SHADERS_SHADER2D_H_

#include <client/shaders/Shader.h>
#include <string>

class Shader2D : public Shader
{
private:
	int uniformModelLocation;
	int uniformOrthoLocation;

public:
	Shader2D(std::string name) : Shader(name), uniformModelLocation(0) {}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif