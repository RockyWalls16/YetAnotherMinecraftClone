/*
 * WindowManager.cpp
 *
 *  Created on: 29 août 2017
 *      Author: valen
 */

#include <client/input/GameController.h>
#include <client/render/WindowManager.h>
#include <client/render/GameRenderer.h>
#include <util/Logger.h>

WindowManager::WindowManager() : gameWindow(nullptr), fullScreen(false), vsync(true)
{}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(gameWindow);
}

int WindowManager::initWindow()
{
	Info("Creating window...");

	// Initialize GLFW
	if (!glfwInit())
	{
		Fatal("Failed to initialize GLFW!");
		return -1;
	}

	// Set Windows parameters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Initialize windows
	GLFWwindow* window = glfwCreateWindow(640, 640, "OGL", nullptr, nullptr);
	if (!window)
	{
		Fatal("Failed to create the game window!");
		return -2;
	}

	glfwMakeContextCurrent(window);
	gameWindow = window;

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(gameWindow, onWindowResize);

	// Enable vsync
	glfwSwapInterval(vsync);

	return 0;
}

void WindowManager::getFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(gameWindow, width, height);
}

void WindowManager::getWindowSize(int* width, int* height)
{
	glfwGetWindowSize(gameWindow, width, height);
}

void WindowManager::swapBuffers()
{
	glfwSwapBuffers(gameWindow);
}

bool WindowManager::shouldClose()
{
	return glfwWindowShouldClose(gameWindow) != 0;
}

GLFWwindow* WindowManager::getGameWindow()
{
	return gameWindow;
}


void WindowManager::setFullScreen(bool fs)
{
	if (fs)
	{
		getWindowSize(&lastWidth, &lastHeight);
		glfwGetWindowPos(gameWindow, &lastX, &lastY);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(gameWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(gameWindow, NULL, lastX, lastY, lastWidth, lastHeight, GLFW_DONT_CARE);
	}

	this->fullScreen = fs;
}

bool WindowManager::isFullScreen()
{
	return fullScreen;
}

void WindowManager::setVsync(bool useVsync)
{
	glfwSwapInterval(useVsync ? 1 : 0);
	vsync = useVsync;
}

bool WindowManager::isVsync()
{
	return vsync;
}

void onWindowResize(GLFWwindow* window, int width, int height)
{
	GameRenderer::getInstance().onResize(width, height);
}

WindowManager* WindowManager::getMainInstance()
{
	return GameRenderer::getInstance().getWindowManager();
}
