/*
 * Framebuffer.cpp
 *
 *  Created on: 12 sept. 2017
 *      Author: Valentin
 */

#include <client/render/util/FrameBuffer.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/Texture.h>
#include <util/Logger.h>
#include <util/GLHeader.h>
#include <client/render/GameRenderer.h>

FrameBuffer* FrameBuffer::makeFBO()
{
	static float vertices[] =
	{
		-1.0F, 1.0F,   //p
		0.0F, 1.0F,    //uv

		-1.0F, -1.0F,  //p
		0.0F, 0.0F,    //uv

		1.0F, -1.0F,   //p
		1.0F, 0.0F,    //uv

		1.0F, 1.0F,    //p
		1.0F, 1.0F     //uv
	};

	static unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Create new frame buffer
	unsigned int fboId;
	glGenFramebuffers(1, &fboId);
	FrameBuffer* framebuffer = new FrameBuffer(fboId);

	VertexArray* sc = VertexArray::makeVAO();
	sc->addVBO(vertices, sizeof(float) * 16, GL_STATIC_DRAW);
	sc->enableEBO(indices, sizeof(unsigned int) * 6, GL_STATIC_DRAW);
	sc->assignPositionAttrib(0, 0, 4 * sizeof(float)); // Position
	sc->assignUVAttrib(0, 1, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
	framebuffer->screenQuad = sc;

	return framebuffer;
}

FrameBuffer::FrameBuffer(unsigned int id) :
		framebufferId(id)
{
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	for (AttachedTexture* attachedTex : attachedTextures)
	{
		delete (attachedTex);
	}

	glDeleteFramebuffers(1, &framebufferId);
}

void FrameBuffer::attachTexture(int type, int width, int height, int internalFormat, int pixelFormat, int dataType)
{
	bind();

	// Load texture and register it as attached
	Texture* texture = TextureLoader::createTexture(NULL, width, height, internalFormat, pixelFormat, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST, dataType);
	AttachedTexture* attachedTexture = new AttachedTexture(texture, type);
	attachedTextures.push_back(attachedTexture);


	// Attach it
	glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture->getTextureId(), 0);
}

void FrameBuffer::attachColorTexture(int width, int height, int colorIndex, int internalType, int pixelFormat, int dataType)
{
	attachTexture(GL_COLOR_ATTACHMENT0 + colorIndex, width, height, internalType, pixelFormat, dataType);
	attachedColors.push_back(GL_COLOR_ATTACHMENT0 + colorIndex);
}

void FrameBuffer::attachDepthTexture(int width, int height)
{
	attachTexture(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);
}

void FrameBuffer::attachDepthBuffer(int width, int height)
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void FrameBuffer::resizeAttachedTexture(int width, int height)
{
	for (AttachedTexture* attachedTex : attachedTextures)
	{
		TextureLoader::reallocateTexture(attachedTex->texture, NULL, false, width, height);
	}

	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void FrameBuffer::bindTexture(int attachedTextureId)
{
	glActiveTexture(GL_TEXTURE0 + attachedTextureId);
	glBindTexture(GL_TEXTURE_2D, attachedTextures[attachedTextureId]->texture->getTextureId());
}

void FrameBuffer::drawOverlay()
{
	screenQuad->drawEBO(6, 0, GL_TRIANGLES, false); // Draw fbo overlay
}

void FrameBuffer::blitFrameBuffer(int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::checkAndUnbind()
{
	glDrawBuffers(attachedColors.size(), attachedColors.data());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Error("FrameBuffer is not complete");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
