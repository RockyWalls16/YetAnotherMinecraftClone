/*
 * GameRenderer.cpp
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#include <client/render/GameRenderer.h>
#include <client/input/GameController.h>
#include <client/render/util/FrameBuffer.h>
#include <util/Logger.h>
#include <client/shaders/ShaderCache.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/TextureCache.h>
#include <client/render/util/VertexArray.h>
#include <client/render/font/FontCache.h>
#include <client/render/font/FontVAO.h>
#include <client/gui/GuiIngame.h>
#include <glm/gtc/matrix_transform.hpp>
#include <math/MathUtil.h>
#include <util/TimeManager.h>
#include <util/GameSettings.h>
#include <Game.h>

GameRenderer::GameRenderer()
{}

void GameRenderer::clearGameRenderer()
{
	// Remove all shaders & textures
	ShaderCache::emptyShaderCache();
	FontCache::cleanUp();
	TextureCache::emptyTextureCache();

	// Destroy renderers
	delete(worldRenderer);
	delete(gBuffer);
}

bool GameRenderer::updateGuiInput()
{
	bool blockInput = false;
	for (Gui* gui : openGuis)
	{
		gui->onInputUpdate(GameController::getInstance().getMouseX() / GUI_SCALE, (frameHeight - GameController::getInstance().getMouseY()) / GUI_SCALE);
		blockInput = blockInput || gui->blockInput();
	}

	return blockInput;
}

void GameRenderer::renderGame()
{
	gBuffer->bind();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	gameCamera.updateCameraRender();

	// Opaque layer
	glDisable(GL_BLEND);
	worldRenderer->render(RenderLayer::RL_OPAQUE);
	
	// SSAO
	ssaoBuffer->bind();
	ShaderCache::ssaoShader->use();
	gBuffer->bindTexture(0); // Bind position buffer
	gBuffer->bindTexture(1); // Bind normal buffer
	ShaderCache::ssaoShader->getNoiseTexture()->bind(2); // Bind noise texture
	ssaoBuffer->drawOverlay();

	// SSAO blur
	blurSSAOBuffer->bind();
	ShaderCache::ssaoBlurShader->use();
	ssaoBuffer->bindTexture(0);
	blurSSAOBuffer->drawOverlay();

	blurSSAOBuffer->unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable wireframe
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDisable(GL_DEPTH_TEST);

	// Light calculation
	ShaderCache::deferredLightingShader->use();
	gBuffer->bindTexture(0);
	gBuffer->bindTexture(1);
	gBuffer->bindTexture(2);
	gBuffer->bindTexture(3);
	blurSSAOBuffer->bindTexture(4, 0);
	gBuffer->bindTexture(5, 4);
	gBuffer->drawOverlay();
	
	// Forward rendering
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	gBuffer->blitFrameBuffer(frameWidth, frameHeight);
	worldRenderer->render(RenderLayer::RL_PRE_PP);
	gameCamera.getCameraRay().tick();
	worldRenderer->render(RenderLayer::RL_TRANSPARENT);


	// UI

	glDisable(GL_DEPTH_TEST);
	// Render guis & update input

	std::vector<Gui*>::iterator it;
	for (it = openGuis.begin(); it != openGuis.end();)
	{
		if ((*it)->shallUiClose())
		{
			delete(*it);
			it = openGuis.erase(it);
		}
		else
		{
			(*it)->render();
			it++;
		}
	}

	checkGLError("Frame");

	LightCache::cleanLights();

	windowManager.swapBuffers();
	
}

int GameRenderer::initGameRenderer()
{
	static bool ready;

	if (!ready)
	{
		ready = true;
		Info("Game renderer initializing...");

		int windowCode = windowManager.initWindow();
		if(windowCode)
		{
			return windowCode;
		}
		
		// Load Shaders
		ShaderCache::initShaderCache();
		FontCache::init();

		gameCamera.initCamera();

		// Set perspective view based on frame buffer size
		int width, height;
		windowManager.getFramebufferSize(&width, &height);
		onResize(width, height);

		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_PROGRAM_POINT_SIZE);

		gBuffer = new FrameBuffer();
		gBuffer->attachColorTexture(width, height, 0, GL_RGB16F, GL_RGB, GL_FLOAT); // Position buffer
		gBuffer->attachColorTexture(width, height, 1, GL_RGBA8_SNORM, GL_RGB, GL_FLOAT); // Normals buffer
		gBuffer->attachColorTexture(width, height, 2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE); // Albedo buffer
		gBuffer->attachColorTexture(width, height, 3, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE); // Light info buffer (spec, spec damper)
		gBuffer->attachDepthTexture(width, height);
		gBuffer->checkAndUnbind();

		// SSAO buffer
		ssaoBuffer = new FrameBuffer();
		ssaoBuffer->attachColorTexture(width, height, 0, GL_RED, GL_RGB, GL_FLOAT);
		ssaoBuffer->checkAndUnbind();

		// Blur SSAO buffer
		blurSSAOBuffer = new FrameBuffer();
		blurSSAOBuffer->attachColorTexture(width, height, 0, GL_RED, GL_RGB, GL_FLOAT);
		blurSSAOBuffer->checkAndUnbind();

		worldRenderer = new WorldRenderer(*Game::getInstance().getWorld());
		
		Gui::initTextures();

		GuiIngame* ingameGui = new GuiIngame();
		ingameGui->open();

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
		orthoProjectionMatrix = glm::ortho(0.0F, (float) width / GUI_SCALE, 0.0F, (float)height / GUI_SCALE, -1.0F, 1.0F);
		gameCamera.setCameraPerspective(60.0F, width, height);
		
		ShaderCache::onResize(width, height);

		if (gBuffer)
		{
			gBuffer->resizeAttachedTexture(width, height);
			ssaoBuffer->resizeAttachedTexture(width, height);
			blurSSAOBuffer->resizeAttachedTexture(width, height);
		}

		for (Gui* gui : openGuis)
		{
			gui->onResize(width / GUI_SCALE, height / GUI_SCALE);
			gui->prepareLayout();
		}
	}
}

WindowManager& GameRenderer::getWindowManager()
{
	return windowManager;
}

Camera& GameRenderer::getGameCamera()
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

std::vector<Gui*>& GameRenderer::getOpenGuis()
{
	return openGuis;
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
