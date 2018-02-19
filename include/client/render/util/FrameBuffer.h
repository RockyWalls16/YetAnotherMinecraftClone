/*
 * Framebuffer.h
 *
 *  Created on: 12 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_UTIL_FRAMEBUFFER_H_
#define CLIENT_RENDER_UTIL_FRAMEBUFFER_H_

#include <client/render/util/VertexArray.h>
#include <vector>


class Texture;

class AttachedTexture
{
public:
	Texture* texture;
	int attachmentType;

	AttachedTexture(Texture* tex, int type) : texture(tex), attachmentType(type) {};
};

class FrameBuffer
{
private:
	unsigned int framebufferId;
	unsigned int rbo;
	std::vector<AttachedTexture*> attachedTextures;
	VertexArray* screenQuad;

public:
	static FrameBuffer* makeFBO();

	void bind();
	void unbind();
	void checkAndUnbind();
	void attachTexture(int type, int width, int height);
	void attachColorTexture(int width, int height, int colorIndex = 0);
	void attachDepthTexture(int width, int height);
	void resizeAttachedTexture(int width, int height);
	void bindTexture(int attachedTextureId);
	void drawOverlay();
	~FrameBuffer();

private:
	FrameBuffer(unsigned int id);
};

#endif /* CLIENT_RENDER_UTIL_FRAMEBUFFER_H_ */
