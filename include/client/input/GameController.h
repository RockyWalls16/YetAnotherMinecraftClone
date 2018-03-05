/*
 * GameController.h
 *
 *  Created on: 31 ao�t 2017
 *      Author: Valentin
 */

#ifndef CLIENT_INPUT_GAMECONTROLLER_H_
#define CLIENT_INPUT_GAMECONTROLLER_H_

#include <util/GLHeader.h>
#include <sparsepp/spp.h>

enum PressMode : char
{
	PRESSED,
	HELD,
	RELEASED,
	NOT_PRESSED
};

enum InputType : char
{
	KEYBOARD,
	MOUSE,
	GAMEPAD // GLFW 3.3
};

class KeyBind
{
public:
	int keyId;
	InputType inputType;
	PressMode keyState;
	PressMode lastState;

	KeyBind(int keyId, InputType inputType);

	bool isPressed()
	{
		return keyState == PressMode::PRESSED;
	}

	bool isReleased()
	{
		return keyState == PressMode::RELEASED;
	}

	bool isHeld()
	{
		return keyState <= PressMode::HELD;
	}

	bool isNotHeld()
	{
		return keyState == PressMode::NOT_PRESSED;
	}
};

class GameController
{
private:
	spp::sparse_hash_map<int, KeyBind*> keyMap;
	bool mouseCaptured;
	int mouseX;
	int mouseY;
	int lastMouseX;
	int lastMouseY;

	// TODO WAIT FOR GLFW 3.3
	/*int gamepadButtonCount;
	const unsigned char* gamepadButtonsStates;*/

public:
	GameController();

	static GameController& getInstance();

	void processInput();
	void updateCameraRotation();
	void setMouseCaptured(bool captured);
	bool isMouseCaptured();
	int getMouseX() const;
	int getMouseY() const;
	void registerKeybind(KeyBind* keybind);

private:
	void updateInputs();
	int actualInputState(KeyBind* keybind);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousePosCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
};

#endif /* CLIENT_INPUT_GAMECONTROLLER_H_ */
