/*
 * GameController.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <Game.h>
#include <client/input/GameController.h>
#include <client/render/WindowManager.h>
#include <client/render/GameRenderer.h>
#include <client/shaders/ShaderCache.h>
#include <util/TimeManager.h>
#include <util/Logger.h>
#include <algorithm>
#include <client/render/ChunkRenderQueue.h>
#include <math/Frustum.h>
#include <glm/vec3.hpp>
#include <core/block/Block.h>
#include <client/render/Camera.h>
#include <client/input/CameraRay.h>
#include <client/gui/GuiPause.h>

KeyBind* GameController::MOUSE_1_KEY;
KeyBind* GameController::MOUSE_2_KEY;
KeyBind* GameController::F1_KEY;
KeyBind* GameController::ESCAPE_KEY;
KeyBind* GameController::F9_KEY;
KeyBind* GameController::F10_KEY;
KeyBind* GameController::F11_KEY;
KeyBind* GameController::F12_KEY;

KeyBind* GameController::W_KEY;
KeyBind* GameController::A_KEY;
KeyBind* GameController::S_KEY;
KeyBind* GameController::D_KEY;
KeyBind* GameController::SHIFT_KEY;
KeyBind* GameController::SPACE_KEY;
KeyBind* GameController::F2_KEY;
KeyBind* GameController::LEFT_KEY;
KeyBind* GameController::RIGHT_KEY;

KeyBind::KeyBind(int keyId, InputType inputType) : keyId(keyId), inputType(inputType)
{
	keyState = lastState = PressMode::NOT_PRESSED;
	GameController::getInstance().registerKeybind(this);
}

GameController::GameController() : mouseCaptured(false)
{
	GLFWwindow* window = WindowManager::getMainInstance().getGameWindow();
	glfwSetKeyCallback(window, keyCallBack);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

GameController& GameController::getInstance()
{
	static GameController instance;
	return instance;
}

void GameController::init()
{
	MOUSE_1_KEY = new KeyBind(GLFW_MOUSE_BUTTON_1, MOUSE);
	MOUSE_2_KEY = new KeyBind(GLFW_MOUSE_BUTTON_2, MOUSE);
	F1_KEY = new KeyBind(GLFW_KEY_F1, KEYBOARD);
	ESCAPE_KEY = new KeyBind(GLFW_KEY_ESCAPE, KEYBOARD);
	F9_KEY = new KeyBind(GLFW_KEY_F9, KEYBOARD);
	F10_KEY = new KeyBind(GLFW_KEY_F10, KEYBOARD);
	F11_KEY = new KeyBind(GLFW_KEY_F11, KEYBOARD);
	F12_KEY = new KeyBind(GLFW_KEY_F12, KEYBOARD);

	W_KEY = new KeyBind(GLFW_KEY_W, KEYBOARD);
	A_KEY = new KeyBind(GLFW_KEY_A, KEYBOARD);
	S_KEY = new KeyBind(GLFW_KEY_S, KEYBOARD);
	D_KEY = new KeyBind(GLFW_KEY_D, KEYBOARD);
	SHIFT_KEY = new KeyBind(GLFW_KEY_LEFT_SHIFT, KEYBOARD);
	SPACE_KEY = new KeyBind(GLFW_KEY_SPACE, KEYBOARD);
	F2_KEY = new KeyBind(GLFW_KEY_F2, KEYBOARD);
	LEFT_KEY = new KeyBind(GLFW_KEY_LEFT, KEYBOARD);
	RIGHT_KEY = new KeyBind(GLFW_KEY_RIGHT, KEYBOARD);
}

void GameController::processInput()
{
	bool blockInput = GameRenderer::getInstance().updateGuiInput();

	if (!blockInput)
	{
		updateCameraRotation();

		static unsigned int selectedBlock = 1;

		Camera& camera = GameRenderer::getInstance().getGameCamera();
		glm::vec3 inputVec = glm::vec3(0.0F, 0.0F, 0.0F);

		if (W_KEY->isHeld())
		{
			inputVec.x += 1.0F;
		}

		if (S_KEY->isHeld())
		{
			inputVec.x -= 1.0F;
		}

		if (A_KEY->isHeld())
		{
			inputVec.z -= 1.0F;
		}

		if (D_KEY->isHeld())
		{
			inputVec.z += 1.0F;
		}

		if (SHIFT_KEY->isHeld())
		{
			inputVec.y -= 1.0F;
		}

		if ((!camera.canRepeatJump() && SPACE_KEY->isPressed()) || (camera.canRepeatJump() && SPACE_KEY->isHeld()))
		{
			inputVec.y += 1.0F;
		}
		camera.inputMove(inputVec);


		// Toggle wireframe
		if (F2_KEY->isPressed())
		{
			GameRenderer::getInstance().setWireFrame(!GameRenderer::getInstance().isWireframeMode());
		}


		// Quit game
		if (ESCAPE_KEY->isPressed())
		{
			GuiPause* pause = new GuiPause();
			pause->open();
		}

		if (LEFT_KEY->isPressed())
		{
			selectedBlock--;
			if (selectedBlock < 1)
			{
				selectedBlock = 1;
			}
		}

		if (RIGHT_KEY->isPressed())
		{
			selectedBlock++;
			if (selectedBlock >= Block::getBlockList().size())
			{
				selectedBlock = Block::getBlockList().size() - 1;
			}
		}

		// Vsync key
		if (F10_KEY->isPressed())
		{
			WindowManager& wm = GameRenderer::getInstance().getWindowManager();
			wm.setVsync(!wm.isVsync());
		}

		// Toggle Fullscreen
		if (F11_KEY->isPressed())
		{
			WindowManager& wm = GameRenderer::getInstance().getWindowManager();
			wm.setFullScreen(!wm.isFullScreen());
		}

		// Reload shaders
		if (F9_KEY->isPressed())
		{
			ShaderCache::initShaderCache();
			int width, height;
			WindowManager::getMainInstance().getFramebufferSize(&width, &height);
			ShaderCache::onResize(width, height);
		}

		// Capture mouse
		if (MOUSE_1_KEY->isPressed())
		{
			if (isMouseCaptured())
			{
				const RaycastResult& lookingBlock = camera.getCameraRay().getLookingBlock();

				if (lookingBlock.blockInfo)
				{
					Game::getInstance().getWorld()->setBlockAt(Block::AIR, lookingBlock.blockInfo->x, lookingBlock.blockInfo->y, lookingBlock.blockInfo->z);
				}
			}

			setMouseCaptured(true);
		}

		if (MOUSE_2_KEY->isPressed())
		{
			if (isMouseCaptured())
			{
				const RaycastResult& lookingBlock = camera.getCameraRay().getLookingBlock();

				if (lookingBlock.blockInfo)
				{
					Game::getInstance().getWorld()->setBlockAt(Block::getBlock(selectedBlock), lookingBlock.blockInfo->x + lookingBlock.nX, lookingBlock.blockInfo->y + lookingBlock.nY, lookingBlock.blockInfo->z + lookingBlock.nZ);
				}
			}

			setMouseCaptured(true);
		}

		// Release mouse
		if (F1_KEY->isPressed())
		{
			setMouseCaptured(false);
		}
	}
	else
	{
		if (isMouseCaptured())
		{
			setMouseCaptured(false);
		}
	}

	updateInputs();
}

void GameController::updateCameraRotation()
{
	if(isMouseCaptured())
	{
		// Calculate mouse movements
		int dX = mouseX - lastMouseX;
		int dY = lastMouseY - mouseY;

		// Apply movement to camera
		Camera& camera = GameRenderer::getInstance().getGameCamera();
		glm::vec3 cameraRotation = camera.getRotation();

		// Create new rotation
		glm::vec3 newRotation = glm::vec3(cameraRotation.x + dY * 0.1F, cameraRotation.y + dX * 0.1F, cameraRotation.z);
		camera.setRotation(newRotation);
	}
}

void GameController::updateInputs()
{
	lastMouseX = mouseX;
	lastMouseY = mouseY;

	// Update gamepad controls
	/*if(glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		gamepadButtonsStates = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &gamepadButtonCount);
	}*/

	for (auto it : keyMap)
	{
		KeyBind* keybind = it.second;
		if (keybind->lastState == PRESSED)
		{
			keybind->keyState = HELD;
		}
		else if (keybind->lastState == RELEASED)
		{
			keybind->keyState = NOT_PRESSED;
		}
		keybind->lastState = keybind->keyState;
	}
}

int GameController::actualInputState(KeyBind* keybind)
{
	switch(keybind->inputType)
	{
		case KEYBOARD:
		{
			return glfwGetKey(WindowManager::getMainInstance().getGameWindow(), keybind->keyId);
		}
		case MOUSE:
		{
			return glfwGetMouseButton(WindowManager::getMainInstance().getGameWindow(), keybind->keyId);
		}
		case GAMEPAD:
		{
			// Handle joystick special case
			/*for(int i = 0; i < gamepadButtonCount; i++)
			{
				if(gamepadButtonsStates[i] == keybind.keyId)
				{
					return gamepadButtonsStates[i];
				}
			}*/

			return GLFW_RELEASE;
		}
		default:
		{
			return GLFW_RELEASE;
		}
	}
}

void GameController::keyCallBack(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	GameController& gc = getInstance();

	auto it = gc.keyMap.find(key);
	KeyBind* keybind = it != gc.keyMap.end() ? it->second : nullptr;

	if (keybind)
	{
		keybind->keyState = keybind->lastState = action == GLFW_PRESS ? PRESSED : (action == GLFW_RELEASE ? RELEASED : HELD);
	}
}

void GameController::mousePosCallback(GLFWwindow * window, double x, double y)
{
	GameController& gc = getInstance();
	gc.mouseX = (int)floor(x);
	gc.mouseY = (int)floor(y);
}

void GameController::mouseButtonCallback(GLFWwindow * window, int key, int action, int mods)
{
	GameController& gc = getInstance();

	auto it = gc.keyMap.find(key);
	KeyBind* keybind = it != gc.keyMap.end() ? it->second : nullptr;

	if (keybind)
	{
		keybind->keyState = keybind->lastState = action == GLFW_PRESS ? PRESSED : (action == GLFW_RELEASE ? RELEASED : HELD);
	}
}

void GameController::setMouseCaptured(bool captured)
{
	glfwSetInputMode(WindowManager::getMainInstance().getGameWindow(), GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	mouseCaptured = captured;
}

bool GameController::isMouseCaptured()
{
	return mouseCaptured;
}


int GameController::getMouseX() const
{
	return mouseX;
}

int GameController::getMouseY() const
{
	return mouseY;
}

void GameController::registerKeybind(KeyBind * keybind)
{
	keyMap[keybind->keyId] = keybind;
}
