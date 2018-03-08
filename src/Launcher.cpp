/*
 * Game.cpp
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#include <ctime>

#include <Game.h>
#include <util/GLHeader.h>
#include <util/Logger.h>


int main(int argc, char* argv[])
{
	Info("Launching game...");

	std::srand(std::time(nullptr));

	int statusCode = Game::getInstance().initGame();
	if(statusCode)
	{
		glfwTerminate();
		return statusCode;
	}

	Game::getInstance().launchGame();

	// Once the game is closed clear everything
	Game::getInstance().clearGame();

	glfwTerminate();

	Info("Game closed.");

	return 0;
}
