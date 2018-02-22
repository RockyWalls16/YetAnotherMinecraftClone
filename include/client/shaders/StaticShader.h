/*
 * StaticShader.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_STATICSHADER_H_
#define CLIENT_SHADERS_STATICSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class StaticShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;
	int uniformNormalLocation;
	int uniformCameraPos;
	int uniformSunDir;
	int uniformAtlasCellSizeLocation;
	int uniformAlbedoTexture;
	int uniformSpecularTexture;
	float cellW;
	float cellH;

public:
	StaticShader(std::string name) : Shader(name), uniformModelLocation(0), uniformViewProjLocation(0), uniformNormalLocation(0), uniformCameraPos(0), uniformSunDir(0), uniformAtlasCellSizeLocation(0), uniformAlbedoTexture(0), uniformSpecularTexture(0), cellW(1.0F), cellH(1.0F){}

	virtual void use() override;
	virtual void stop() override;
	void setAtlasCellSize(float cellW, float cellH);
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif /* CLIENT_SHADERS_STATICSHADER_H_ */
