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

KeyBind::KeyBind(int keyId, InputType inputType) : keyId(keyId), inputType(inputType)
{
	keyState = lastState = PressMode::NOT_PRESSED;
	GameController::getInstance().registerKeybind(this);
}

GameController& GameController::getInstance()
{
	static GameController instance;
	return instance;
}

void GameController::processInput()
{
	updateCameraRotation();

	static KeyBind* MOUSE_KEY = new KeyBind(GLFW_MOUSE_BUTTON_1, MOUSE);
	static KeyBind* F1_KEY = new KeyBind(GLFW_KEY_F1, KEYBOARD);
	static KeyBind* ESCAPE_KEY = new KeyBind(GLFW_KEY_ESCAPE, KEYBOARD);
	static KeyBind* F9_KEY = new KeyBind(GLFW_KEY_F9, KEYBOARD);
	static KeyBind* F10_KEY = new KeyBind(GLFW_KEY_F10, KEYBOARD);
	static KeyBind* F11_KEY = new KeyBind(GLFW_KEY_F11, KEYBOARD);
	static KeyBind* F12_KEY = new KeyBind(GLFW_KEY_F12, KEYBOARD);

	static KeyBind* W_KEY = new KeyBind(GLFW_KEY_W, KEYBOARD);
	static KeyBind* A_KEY = new KeyBind(GLFW_KEY_A, KEYBOARD);
	static KeyBind* S_KEY = new KeyBind(GLFW_KEY_S, KEYBOARD);
	static KeyBind* D_KEY = new KeyBind(GLFW_KEY_D, KEYBOARD);
	static KeyBind* SHIFT_KEY = new KeyBind(GLFW_KEY_LEFT_SHIFT, KEYBOARD);
	static KeyBind* SPACE_KEY = new KeyBind(GLFW_KEY_SPACE, KEYBOARD);
	static KeyBind* F2_KEY = new KeyBind(GLFW_KEY_F2, KEYBOARD);

	Camera* camera = GameRenderer::getInstance().getGameCamera();
	glm::vec3 inputVec = glm::vec3(0.0F, 0.0F, 0.0F);

	if(W_KEY->isHeld())
	{
		inputVec.x += 1.0F;
	}

	if(S_KEY->isHeld())
	{
		inputVec.x -= 1.0F;
	}

	if(A_KEY->isHeld())
	{
		inputVec.z -= 1.0F;
	}

	if(D_KEY->isHeld())
	{
		inputVec.z += 1.0F;
	}

	if(SHIFT_KEY->isHeld())
	{
		inputVec.y -= 1.0F;
	}

	if((!camera->canRepeatJump() && SPACE_KEY->isPressed()) || (camera->canRepeatJump() && SPACE_KEY->isHeld()))
	{
		inputVec.y += 1.0F;
	}
	camera->inputMove(inputVec);


	// Toggle wireframe
	if(F2_KEY->isPressed())
	{
		GameRenderer::getInstance().setWireFrame(!GameRenderer::getInstance().isWireframeMode());
	}


	// Quit game
	if (ESCAPE_KEY->isPressed())
	{
		Game::getInstance().closeGame();
	}
	
	// Test key
	if (F9_KEY->isPressed())
	{
		
	}

	// Vsync key
	if (F10_KEY->isPressed())
	{
		WindowManager* wm = GameRenderer::getInstance().getWindowManager();
		wm->setVsync(!wm->isVsync());
	}

	// Toggle Fullscreen
	if (F11_KEY->isPressed())
	{
		WindowManager* wm = GameRenderer::getInstance().getWindowManager();
		wm->setFullScreen(!wm->isFullScreen());
	}

	// Reload shaders
	if (F12_KEY->isPressed())
	{
		ShaderCache::initShaderCache();
	}

	// Capture mouse
	if (MOUSE_KEY->isPressed())
	{
		setMouseCaptured(true);
	}

	// Release mouse
	if (F1_KEY->isPressed())
	{
		setMouseCaptured(false);
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
		Camera* camera = GameRenderer::getInstance().getGameCamera();
		glm::vec3 cameraRotation = camera->getRotation();

		// Create new rotation
		glm::vec3 newRotation = glm::vec3(cameraRotation.x + dY * 0.1F, cameraRotation.y + dX * 0.1F, cameraRotation.z);
		camera->setRotation(newRotation);
	}
}

void GameController::updateInputs()
{
	lastMouseX = mouseX;
	lastMouseY = mouseY;

	// Set mouse pos
	double xPos;
	double yPos;

	glfwGetCursorPos(WindowManager::getMainInstance()->getGameWindow(), &xPos, &yPos);
	mouseX = (int) floor(xPos);
	mouseY = (int) floor(yPos);

	// Update gamepad controls
	/*if(glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		gamepadButtonsStates = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &gamepadButtonCount);
	}*/

	for (auto it : keyMap)
	{
		KeyBind* keybind = it.second;
		
		if (actualInputState(keybind) == GLFW_PRESS)
		{
			if (keybind->lastState == PRESSED || keybind->lastState == HELD)
			{
				keybind->keyState = HELD;
			}
			else
			{
				keybind->keyState = PRESSED;
			}
		}
		else
		{
			if (keybind->lastState == PRESSED || keybind->lastState == HELD)
			{
				keybind->keyState = RELEASED;
			}
			else
			{
				keybind->keyState = NOT_PRESSED;
			}
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
			return glfwGetKey(WindowManager::getMainInstance()->getGameWindow(), keybind->keyId);
		}
		case MOUSE:
		{
			return glfwGetMouseButton(WindowManager::getMainInstance()->getGameWindow(), keybind->keyId);
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

void GameController::setMouseCaptured(bool captured)
{
	glfwSetInputMode(WindowManager::getMainInstance()->getGameWindow(), GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool GameController::isMouseCaptured()
{
	return glfwGetInputMode(WindowManager::getMainInstance()->getGameWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
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
