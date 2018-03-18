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

DeferredBlockShader* ShaderCache::deferredBlockShader = ShaderLoader::registerShader<DeferredBlockShader>("deferred_block");
ForwardBlockShader* ShaderCache::forwardBlockShader = ShaderLoader::registerShader<ForwardBlockShader>("forward_block");
DeferredLightingShader* ShaderCache::deferredLightingShader = ShaderLoader::registerShader<DeferredLightingShader>("PP/deferred_light");
SSAOShader* ShaderCache::ssaoShader = ShaderLoader::registerShader<SSAOShader>("PP/ssao");
BlurShader* ShaderCache::ssaoBlurShader = ShaderLoader::registerShader<BlurShader>("PP/blur");
Shader2D* ShaderCache::shader2d = ShaderLoader::registerShader<Shader2D>("2d");
SkySphereShader* ShaderCache::skyShader = ShaderLoader::registerShader<SkySphereShader>("skySphere");
StarShader* ShaderCache::starShader = ShaderLoader::registerShader<StarShader>("stars");
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

void ShaderCache::onResize(int width, int height)
{
	for (Shader* shader : ShaderCache::shaderCache)
	{
		shader->onResize(width, height);
	}
}

void ShaderCache::addShader(Shader& shader)
{
	ShaderCache::shaderCache.push_back(&shader);
}
