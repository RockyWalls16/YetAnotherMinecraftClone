#ifndef CLIENT_SHADERS_SKYSHADER_H_
#define CLIENT_SHADERS_SKYSHADER_H_

#include <client/shaders/Shader.h>
#include <util/Color.h>
#include <string>

class SkySphereShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;
	int uniformTopColorLocation;
	int uniformBottomColorLocation;

public:
	SkySphereShader(std::string name) : Shader(name) {}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void use() override;
	void setColors(Color& topColor, Color& bottomColor);
private:
	virtual void bindAttributesAndUniforms() override;
};


#endif