/*
 * Texture.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_TEXTURES_TEXTURE_H_
#define CLIENT_TEXTURES_TEXTURE_H_

class Texture
{
private:
	unsigned int textureId;
	unsigned int textureWidth;
	unsigned int textureHeight;
	int textureInternalFormat;
	int texturePixelDataFormat;

public:
	Texture(unsigned int textureId, unsigned int width, unsigned int height, int internalFormat, int pixelDataFormat);
	~Texture();

	void bind(int id = 0);
	void unbind(int id = 0);
	unsigned int getTextureId() const;
	unsigned int getTextureWidth() const;
	unsigned int getTextureHeight() const;
	int getTextureInternalFormat() const;
	int getTexturePixelDataFormat() const;
};

#endif /* CLIENT_TEXTURES_TEXTURE_H_ */
