/*
 * ShaderCache.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_SHADERS_SHADERCACHE_H_
#define CLIENT_SHADERS_SHADERCACHE_H_

#include <client/shaders/Shader.h>
#include <client/shaders/DeferredBlockShader.h>
#include <client/shaders/PostProcessShader.h>
#include <client/shaders/SkySphereShader.h>
#include <client/shaders/StarShader.h>
#include <client/shaders/Shader2D.h>
#include <client/shaders/UnlitShader.h>
#include <client/shaders/ForwardBlockShader.h>
#include <vector>

class ShaderCache
{
public:
	static DeferredBlockShader* deferredBlockShader;
	static ForwardBlockShader* forwardBlockShader;
	static PostProcessShader* postShader;
	static Shader2D* shader2d;
	static SkySphereShader* skyShader;
	static StarShader* starShader;
	static UnlitShader* unlitShader;

private:
	static std::vector<Shader*> shaderCache;

public:

	static void initShaderCache();
	static void emptyShaderCache();

	static void addShader(Shader& shader);
};

#endif /* CLIENT_SHADERS_SHADERCACHE_H_ */
