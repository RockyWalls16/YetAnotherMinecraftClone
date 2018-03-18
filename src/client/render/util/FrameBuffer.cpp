/*
 * Framebuffer.cpp
 *
 *  Created on: 12 sept. 2017
 *      Author: Valentin
 */

#include <client/render/util/FrameBuffer.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/Texture.h>
#include <client/shaders/Shader.h>
#include <util/Logger.h>
#include <util/GLHeader.h>
#include <client/render/GameRenderer.h>

FrameBuffer::FrameBuffer()
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

	// Register new frame buffer
	glGenFramebuffers(1, &framebufferId);
	screenQuad.addVBO(vertices, sizeof(float) * 16, GL_STATIC_DRAW);
	screenQuad.enableEBO(indices, sizeof(unsigned int) * 6, GL_STATIC_DRAW);
	screenQuad.assignPositionAttrib(0, 0, 4 * sizeof(float)); // Position
	screenQuad.assignUVAttrib(0, 1, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
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

void FrameBuffer::attachColorTexture(int colorIndex, Texture * texture)
{
	AttachedTexture* attachedTexture = new AttachedTexture(texture, GL_COLOR_ATTACHMENT0 + colorIndex);
	attachedTextures.push_back(attachedTexture);

	// Attach it
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, texture->getTextureId(), 0);
	attachedColors.push_back(GL_COLOR_ATTACHMENT0 + colorIndex);
}

void FrameBuffer::attachDepthTexture(int width, int height)
{
	attachTexture(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
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

void FrameBuffer::bindTexture(int glTextureId, int attachedTextureId)
{
	glActiveTexture(GL_TEXTURE0 + glTextureId);
	glBindTexture(GL_TEXTURE_2D, attachedTextures[attachedTextureId]->texture->getTextureId());
}

void FrameBuffer::drawOverlay()
{
	screenQuad.drawEBO(6, 0, GL_TRIANGLES, false); // Draw fbo overlay
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
