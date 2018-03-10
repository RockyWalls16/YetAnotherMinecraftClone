/*
 * WindowManager.h
 *
 *  Created on: 29 août 2017
 *      Author: valen
 */

#ifndef CLIENT_RENDER_WINDOWMANAGER_H_
#define CLIENT_RENDER_WINDOWMANAGER_H_

#include <util/GLHeader.h>

class WindowManager
{
private:
	GLFWwindow* gameWindow;
	bool fullScreen;
	bool vsync;
	int lastWidth;
	int lastHeight;
	int lastX;
	int lastY;

public:
	WindowManager();
	~WindowManager();

	int initWindow();

	void getFramebufferSize(int* width, int* height);

	void getWindowSize(int* width, int* height);

	void swapBuffers();

	bool shouldClose();

	GLFWwindow* getGameWindow();

	void setFullScreen(bool fullscreen);

	bool isFullScreen();

	void setVsync(bool useVsync);

	bool isVsync();

	static WindowManager& getMainInstance();
};

void onWindowResize(GLFWwindow* window, int width, int height);

#endif /* CLIENT_RENDER_WINDOWMANAGER_H_ */
