/*
 * ShaderCache.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <client/shaders/ShaderCache.h>
#include <client/shaders/ShaderLoader.h>
#include <util/Logger.h>

std::vector<Shader*> ShaderCache::shaderCache;

StaticShader* ShaderCache::blockShader = ShaderLoader::registerShader<StaticShader>("block");
PostProcessShader* ShaderCache::postShader = ShaderLoader::registerShader<PostProcessShader>("post_process");
Shader2D* ShaderCache::shader2d = ShaderLoader::registerShader<Shader2D>("2d");
SkyShader* ShaderCache::skyShader = ShaderLoader::registerShader<SkyShader>("skySphere");
UnlitShader* ShaderCache::unlitShader = ShaderLoader::registerShader<UnlitShader>("unlit");

void ShaderCache::initShaderCache()
{
	// Loads every shaders into memory
	int shaderAmount = ShaderCache::shaderCache.size();
	for(int i = 0; i < shaderAmount; i++)
	{
		Shader* shader = ShaderCache::shaderCache[i];

		Info("Initializing shader: " + shader->getShaderName() + " (" + std::to_string(i + 1) + "/" + std::to_string(shaderAmount) + ")");
		ShaderLoader::initShader(*shader);
	}
}

void ShaderCache::emptyShaderCache()
{
	Info("Removing shaders...");

	// Remove all shaders from memory
	for(Shader* shader : ShaderCache::shaderCache)
	{
		delete(shader);
	}
	ShaderCache::shaderCache.clear();
}

void ShaderCache::addShader(Shader& shader)
{
	ShaderCache::shaderCache.push_back(&shader);
}
