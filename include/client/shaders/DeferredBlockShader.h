/*
 * DeferredBlockShader.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_DEFERREDBLOCKSHADER_H_
#define CLIENT_SHADERS_DEFERREDBLOCKSHADER_H_

#include <client/shaders/Shader.h>
#include <string>

class DeferredBlockShader : public Shader
{
private:
	int uniformModelLocation;
	int uniformViewProjLocation;
	int uniformNormalLocation;
	int uniformAtlasCellSizeLocation;
	int uniformAlbedoTexture;
	int uniformSpecularTexture;
	float cellW;
	float cellH;

public:
	DeferredBlockShader(std::string name) : Shader(name), cellW(1.0F), cellH(1.0F){}

	virtual void use() override;
	virtual void stop() override;
	void setAtlasCellSize(float cellW, float cellH);
	virtual void onDraw(glm::mat4& modelMatrix, glm::mat3& normalMatrix) override;

private:
	virtual void bindAttributesAndUniforms() override;
};

#endif /* CLIENT_SHADERS_DEFERREDBLOCKSHADER_H_ */
