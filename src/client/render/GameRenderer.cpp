/*
 * GameRenderer.cpp
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#include <client/render/GameRenderer.h>
#include <client/render/util/FrameBuffer.h>
#include <util/Logger.h>
#include <client/shaders/ShaderCache.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/TextureCache.h>
#include <client/render/util/VertexArray.h>
#include <client/render/font/FontCache.h>
#include <client/render/font/FontVAO.h>
#include <client/render/font/FontRenderer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <util/TimeManager.h>
#include <Game.h>

void GameRenderer::clearGameRenderer()
{
	// Remove all shaders & textures
	ShaderCache::emptyShaderCache();
	FontCache::cleanUp();
	TextureCache::emptyTextureCache();

	// Destroy renderers
	delete(worldRenderer);

	// Destroy window
	delete(windowManager);
	delete(frameBuffer);

	delete(gameCamera);
}

void GameRenderer::renderGame()
{
	static Font* testFont;
	static FontVAO* fvao;
	static FontVAO* coords;
	if (testFont == nullptr)
	{
		testFont = FontCache::loadFont("default");
		fvao = FontRenderer::makeVao(testFont, "FPS: ");
	}

	if (TimeManager::fps == 0)
	{ 
		delete(fvao);
		fvao = FontRenderer::makeVao(testFont, "FPS: " + std::to_string(TimeManager::lastFps));
	}

	if (coords != nullptr)
	{
		delete(coords);
	}
	//coords = FontRenderer::makeVao(testFont, "X: " + std::to_string((int) gameCamera->getLocation().x) + " Y: " + std::to_string((int)gameCamera->getLocation().y) + " Z: " + std::to_string((int)gameCamera->getLocation().z));

	frameBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	gameCamera->updateCameraRender();

	// Opaque layer
	glDisable(GL_BLEND);
	worldRenderer->render(RenderLayer::RL_OPAQUE);
	
	// Unbind framebuffer
	frameBuffer->unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable wireframe
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	worldRenderer->render(RenderLayer::RL_PRE_PP);

	glDisable(GL_DEPTH_TEST);
	ShaderCache::postShader->use();
	frameBuffer->bindTexture(0);
	frameBuffer->bindTexture(1);
	frameBuffer->bindTexture(2);
	frameBuffer->bindTexture(3);
	frameBuffer->bindTexture(4);
	frameBuffer->drawOverlay();
	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	frameBuffer->blitFrameBuffer(frameWidth, frameHeight);
	gameCamera->getCameraRay().tick();
	worldRenderer->render(RenderLayer::RL_TRANSPARENT);


	// UI

	glDisable(GL_DEPTH_TEST);
	fvao->render2D(4, 28);
	//coords->render2D(4, 58);

	//checkGLError("Frame");

	windowManager->swapBuffers();
	
}

int GameRenderer::initGameRenderer()
{
	static bool ready;

	if (!ready)
	{
		ready = true;
		Info("Game renderer initializing...");

		WindowManager* wM = new WindowManager();
		int windowCode = wM->initWindow();
		if(windowCode)
		{
			return windowCode;
		}
		windowManager = wM;

		// Load Shaders
		ShaderCache::initShaderCache();

		gameCamera = new Camera();

		// Set perspective view based on frame buffer size
		int width, height;
		windowManager->getFramebufferSize(&width, &height);
		onResize(width, height);

		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		frameBuffer = FrameBuffer::makeFBO();
		frameBuffer->attachColorTexture(width, height, 0, GL_RGB16F, GL_RGB, GL_FLOAT); // Position buffer
		frameBuffer->attachColorTexture(width, height, 1, GL_RGB10_A2, GL_RGB, GL_FLOAT); // Normals buffer
		frameBuffer->attachColorTexture(width, height, 2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE); // Albedo buffer
		frameBuffer->attachColorTexture(width, height, 3, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE); // Light info buffer (spec, spec damper)
		frameBuffer->attachDepthTexture(width, height);
		//frameBuffer->attachDepthBuffer(width, height);
		frameBuffer->checkAndUnbind();

		worldRenderer = new WorldRenderer(Game::getInstance().getWorld());

		return 0;
	}

	Error("Game renderer already initialized!");

	return 1;
}

void GameRenderer::onResize(int width, int height)
{
	// Handle game resize
	if(width > 0 && height > 0)
	{
		glViewport(0, 0, width, height);
		frameWidth = width;
		frameHeight = height;

		// Update matrices
		orthoProjectionMatrix = glm::ortho(0.0F, (float) width, 0.0F, (float)height, -1.0F, 1.0F);
		gameCamera->setCameraPerspective(70.0F, width, height);
		
		ShaderCache::postShader->onResize(width, height);

		if (frameBuffer)
		{
			frameBuffer->resizeAttachedTexture(width, height);
		}
	}
}

WindowManager* GameRenderer::getWindowManager()
{
	return windowManager;
}

Camera* GameRenderer::getGameCamera()
{
	return gameCamera;
}

WorldRenderer* GameRenderer::getWorldRenderer()
{
	return worldRenderer;
}

bool GameRenderer::isWireframeMode()
{
	return wireframe;
}

void GameRenderer::setWireFrame(bool wireframe)
{
	this->wireframe = wireframe;
}

glm::mat4 GameRenderer::getOrthoMatrix()
{
	return orthoProjectionMatrix;
}

void GameRenderer::checkGLError(std::string tag)
{
	GLenum error = glGetError();

	if (error != 0)
	{
		Error("GL Error at : " + tag);
		switch (error)
		{
		case GL_INVALID_VALUE: Error("Opengl error (GL_INVALID_VALUE) for call "); break;
		case GL_INVALID_ENUM: Error("Opengl error (GL_INVALID_ENUM) for call "); break;
		case GL_INVALID_OPERATION: Error("Opengl error (GL_INVALID_OPERATION) for call "); break;
		case GL_STACK_OVERFLOW: Error("Opengl error (GL_STACK_OVERFLOW) for call "); break;
		case GL_STACK_UNDERFLOW: Error("Opengl error (GL_STACK_UNDERFLOW) for call "); break;
		case GL_OUT_OF_MEMORY: Error("Opengl error (GL_OUT_OF_MEMORY) for call "); break;
		case GL_TABLE_TOO_LARGE: Error("Opengl error (GL_TABLE_TOO_LARGE) for call "); break;
		default: Error("Unknown Opengl error for call "); break;
		}
	}
}

GameRenderer& GameRenderer::getInstance()
{
	static GameRenderer instance;
	return instance;
}
