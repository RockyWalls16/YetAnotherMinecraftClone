/*
 * GameController.h
 *
 *  Created on: 31 août 2017
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

	static KeyBind* MOUSE_1_KEY;
	static KeyBind* MOUSE_2_KEY;
	static KeyBind* F1_KEY;
	static KeyBind* ESCAPE_KEY;
	static KeyBind* F9_KEY;
	static KeyBind* F10_KEY;
	static KeyBind* F11_KEY;
	static KeyBind* F12_KEY;

	static KeyBind* W_KEY;
	static KeyBind* A_KEY;
	static KeyBind* S_KEY;
	static KeyBind* D_KEY;
	static KeyBind* E_KEY;
	static KeyBind* SHIFT_KEY;
	static KeyBind* SPACE_KEY;
	static KeyBind* F2_KEY;
	static KeyBind* LEFT_KEY;
	static KeyBind* RIGHT_KEY;

	GameController();

	void init();
	void processInput();
	void updateCameraRotation();
	void setMouseCaptured(bool captured);
	bool isMouseCaptured();
	int getMouseX() const;
	int getMouseY() const;
	void registerKeybind(KeyBind* keybind);

	static GameController& getInstance();

private:
	void updateInputs();
	int actualInputState(KeyBind* keybind);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mousePosCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif /* CLIENT_INPUT_GAMECONTROLLER_H_ */
