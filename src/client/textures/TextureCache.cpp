/*
 * TextureCache.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#include <client/textures/TextureCache.h>
#include <client/textures/TextureLoader.h>
#include <util/Logger.h>

std::vector<Texture*> TextureCache::textureCache;
Texture* TextureCache::blockTexture;
Texture* TextureCache::blockSpecularTexture;

void TextureCache::emptyTextureCache()
{
	Info("Emptying the texture cache...");

	for(Texture* texture : textureCache)
	{
		delete(texture);
	}
	textureCache.clear();
}

void TextureCache::addTexture(Texture* texture)
{
	textureCache.push_back(texture);
}
