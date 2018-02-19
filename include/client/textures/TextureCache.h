/*
 * TextureCache.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_TEXTURES_TEXTURECACHE_H_
#define CLIENT_TEXTURES_TEXTURECACHE_H_

#include <client/textures/Texture.h>
#include <vector>


class TextureCache
{
public:
	static Texture* blockTexture;

private:
	static std::vector<Texture*> textureCache;

public:
	static void emptyTextureCache();

	static void addTexture(Texture* texture);
};

#endif /* CLIENT_TEXTURES_TEXTURECACHE_H_ */
