/*
 * TextureLoader.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#include <util/Logger.h>
#include <stb_image.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/TextureCache.h>

Texture* TextureLoader::loadTexture(const std::string& path, bool useMipmap, int repeatModeS, int repeatModeT, int minFilter, int maxFilter)
{
	Debug("Loading: " + path);


	int width;
	int height;
	int nrChannels;

	// Load from disk
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load((TEXTURE_PATH + path + ".png").c_str(), &width, &height, &nrChannels, 0);
	bool success = true;

	// Error management
	if (!data)
	{
		success = false;
	    NLog(LOG_LINE);
	    Error("Failed to load texture (" + path + "):");
	    NLog(std::string(stbi_failure_reason()) + "\n");
	    NLog(LOG_LINE);
	    Warning("Using default error texture for (" + path + ")");

	    // Load error texture
	    unsigned char errorTexture[] = {
	    		255, 255, 255, 255,
				128, 128, 128, 255,
				128, 128, 128, 255,
				255, 255, 255, 255
	    };

		minFilter = maxFilter = GL_NEAREST;
		width = height = 2;
		data = errorTexture;
	}

	int pixelFormat = GL_RGB;
	if(nrChannels == 4)
	{
		pixelFormat = GL_RGBA;
	}
	else if(nrChannels == 1)
	{
		pixelFormat = GL_RED;
	}

	Texture* output = createTexture(data, width, height, pixelFormat, pixelFormat, useMipmap, repeatModeS, repeatModeT, minFilter, maxFilter);

	if(success)
	{
		// Auto generate mipmap if needed
		if(useMipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Free memory
		stbi_image_free(data);
	}

	return output;
}

Texture* TextureLoader::createTexture(void* data, int width, int height, int internalFormat, int pixelDataFormat, bool useMipmap, int repeatModeS, int repeatModeT, int minFilter, int maxFilter)
{
	// Generate new texture
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	Texture* output = new Texture(textureId, width, height, internalFormat, pixelDataFormat);

	// Add texture data
	reallocateTexture(output, data, width, height);

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatModeT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

	// Register texture into cache
	TextureCache::addTexture(output);


	return output;
}

void TextureLoader::reallocateTexture(Texture* texture, void* data, int width, int height)
{
	texture->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, texture->getTextureInternalFormat(), width, height, 0, texture->getTexturePixelDataFormat(), GL_UNSIGNED_BYTE, data);
}
