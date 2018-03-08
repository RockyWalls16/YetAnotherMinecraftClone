#ifndef CLIENT_SHADERS_FORWARDBLOCKSHADER_H_
#define CLIENT_SHADERS_FORWARDBLOCKSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class ForwardBlockShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;
	int uniformNormalLocation;
	int uniformAtlasCellSizeLocation;
	int uniformAlbedoTexture;
	int uniformSpecularTexture;
	int uniformSunColorLocation;
	int uniformSunDirLocation;
	int uniformCameraPosLocation;

	float cellW;
	float cellH;

public:
	ForwardBlockShader(std::string name) : Shader(name), uniformModelLocation(0), uniformViewProjLocation(0), uniformNormalLocation(0), uniformAtlasCellSizeLocation(0), uniformAlbedoTexture(0), uniformSpecularTexture(0), uniformSunDirLocation(0), uniformSunColorLocation(0), uniformCameraPosLocation(0), cellW(1.0F), cellH(1.0F) {}

	virtual void use() override;
	virtual void stop() override;
	void setAtlasCellSize(float cellW, float cellH);
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif
