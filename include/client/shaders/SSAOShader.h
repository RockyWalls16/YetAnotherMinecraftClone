#ifndef CLIENT_SHADERS_SSAOSHADER_H_
#define CLIENT_SHADERS_SSAOSHADER_H_

#include <client/shaders/Shader.h>
#include <client/textures/Texture.h>
#include <string>

class SSAOShader : public Shader
{
private:
	Texture * noiseTexture;
	int uniformProjectionLocation;
	int uniformNoiseScaleLocation;

public:
	SSAOShader(std::string name) : Shader(name) {}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void onResize(int width, int height) override;
	Texture* getNoiseTexture();

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif