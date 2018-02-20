/*
 * TextureLoader.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_TEXTURES_TEXTURELOADER_H_
#define CLIENT_TEXTURES_TEXTURELOADER_H_

#include <util/GLHeader.h>
#include <string>

class Texture;

#define TEXTURE_PATH "Assets/Textures/"
#define DEFAULT_MIN_FILTER GL_NEAREST
#define DEFAULT_MAX_FILTER GL_NEAREST

class TextureLoader
{
public:
	static Texture* loadTexture(const std::string& path, bool useMipmap = false, int repeatModeS = GL_REPEAT, int repeatModeT = GL_REPEAT, int minFilter = DEFAULT_MIN_FILTER, int maxFilter = DEFAULT_MAX_FILTER);

	static Texture* createTexture(void* data, int width, int height, int internalFormat, int pixelDataFormat, bool useMipmap = false, int repeatModeS = GL_REPEAT, int repeatModeT = GL_REPEAT, int minFilter = DEFAULT_MIN_FILTER, int maxFilter = DEFAULT_MAX_FILTER);

	static void reallocateTexture(Texture* texture, void* data, bool useMipmap, int width, int height);
};

#endif /* CLIENT_TEXTURES_TEXTURELOADER_H_ */
