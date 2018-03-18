#ifndef CLIENT_SHADERS_BLURSHADER_H_
#define CLIENT_SHADERS_BLURSHADER_H_

#include <client/shaders/Shader.h>
#include <client/textures/Texture.h>
#include <string>

class BlurShader : public Shader
{
private:
	int pixelSizeLocation;

public:
	BlurShader(std::string name) : Shader(name) {}
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;
	virtual void onResize(int width, int height) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif