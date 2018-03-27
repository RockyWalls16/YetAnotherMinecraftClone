/*
 * GameRenderer.h
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#ifndef GAMERENDERER_H_
#define GAMERENDERER_H_

#include <client/render/Camera.h>
#include <client/render/util/FrameBuffer.h>
#include <client/render/WindowManager.h>
#include <client/render/WorldRenderer.h>
#include <client/gui/Gui.h>
#include <vector>

enum RenderLayer
{
	RL_OPAQUE,
	RL_TRANSPARENT,
	RL_PRE_PP,
	RL_POST_PP
};

class GameRenderer
{
private:
	int frameWidth;
	int frameHeight;

	WindowManager windowManager;
	Camera gameCamera;
	std::vector<Gui*> openGuis;
	WorldRenderer* worldRenderer;
	FrameBuffer* gBuffer;
	FrameBuffer* ssaoBuffer;
	FrameBuffer* blurSSAOBuffer;
	glm::mat4 orthoProjectionMatrix;
	bool wireframe;

public:
	GameRenderer();
	int initGameRenderer();
	void renderGame();

	void onResize(int width, int height);

	void clearGameRenderer();

	bool updateGuiInput();

	WindowManager& getWindowManager();
	
	static GameRenderer& getInstance();
	Camera& getGameCamera();
	WorldRenderer* getWorldRenderer();

	bool isWireframeMode();
	void setWireFrame(bool wireframe);
	void setFov(float fov);

	std::vector<Gui*>& getOpenGuis();
	glm::mat4 getOrthoMatrix();


	static void checkGLError(std::string tag); 
};

#endif /* GAMERENDERER_H_ */
