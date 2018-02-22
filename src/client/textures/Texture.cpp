/*
 * Texture.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#include <client/textures/Texture.h>
#include <util/GLHeader.h>

Texture::Texture(unsigned int id, unsigned int width, unsigned int height, int internalFormat, int pixelDataFormat) :
		textureId(id), textureWidth(width), textureHeight(height), textureInternalFormat(internalFormat), texturePixelDataFormat(pixelDataFormat)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

unsigned int Texture::getTextureId() const
{
	return textureId;
}

unsigned int Texture::getTextureWidth() const
{
	return textureWidth;
}

unsigned int Texture::getTextureHeight() const
{
	return textureHeight;
}

void Texture::bind(int id)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind(int id)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getTextureInternalFormat() const
{
	return textureInternalFormat;
}

int Texture::getTexturePixelDataFormat() const
{
	return texturePixelDataFormat;
}
