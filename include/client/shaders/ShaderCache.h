/*
 * ShaderCache.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_SHADERCACHE_H_
#define CLIENT_SHADERS_SHADERCACHE_H_

#include <client/shaders/Shader.h>
#include <client/shaders/StaticShader.h>
#include <client/shaders/PostProcessShader.h>
#include <client/shaders/SkyShader.h>
#include <client/shaders/Shader2D.h>
#include <client/shaders/UnlitShader.h>
#include <vector>

class ShaderCache
{
public:
	static StaticShader* blockShader;
	static PostProcessShader* testPostShader;
	static Shader2D* shader2d;
	static SkyShader* skyShader;
	static UnlitShader* unlitShader;

private:
	static std::vector<Shader*> shaderCache;

public:

	static void initShaderCache();
	static void emptyShaderCache();

	static void addShader(Shader& shader);
};

#endif /* CLIENT_SHADERS_SHADERCACHE_H_ */
