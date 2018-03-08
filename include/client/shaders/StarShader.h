#ifndef CLIENT_SHADERS_STARSHADER_H_
#define CLIENT_SHADERS_STARSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class StarShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;
	int uniformStarAlphaLocation;

public:
	StarShader(std::string name) : Shader(name), uniformModelLocation(0), uniformViewProjLocation(0), uniformStarAlphaLocation(0) {}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void use() override;
	void setStarAlpha(float alpha);

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif