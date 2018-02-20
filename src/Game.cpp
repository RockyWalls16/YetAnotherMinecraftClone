/*
 * Game.cpp
 *
 *  Created on: 29 ao�t 2017
 *      Author: Valentin
 */

#include <Game.h>
#include <core/world/World.h>
#include <core/entity/Entity.h>
#include <core/entity/EntityPlayer.h>
#include <client/render/GameRenderer.h>
#include <client/input/GameController.h>
#include <util/TimeManager.h>
#include <util/Logger.h>
#include <string>

int Game::initGame()
{
	static bool launched;

	if(!launched)
	{
		Info("Initializing game...");
		launched = true;

		world = new World();
		player = new EntityPlayer(*world);
		player->setRotation(glm::vec3(0.0F, 0.0F, -1.0F));
		player->setPosition(glm::vec3(20.0F, 91.0F, 20.0F));

		//Try to initialize game renderer
		int rendererStatus = GameRenderer::getInstance().initGameRenderer();
		if (rendererStatus)
		{
			return rendererStatus;
		}

		GameRenderer::getInstance().getGameCamera()->setTrackedEntity(*player);

		return 0;
	}

	Error("Game already launched!");

	return 1;
}

void Game::launchGame()
{
	TimeManager::initTimer();

	while (!shouldExit())
	{
		TimeManager::updateTimer();

		GameController::getInstance().processInput();

		while(TimeManager::shallTick())
		{
			//Update game logic
			world->tick();
		}

		GameRenderer::getInstance().renderGame();

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void Game::closeGame()
{
	closeAsked = true;
}

void Game::clearGame()
{
	Info("Game closing. Cleaning up...");
	GameRenderer::getInstance().clearGameRenderer();
}

bool Game::shouldExit()
{
	return GameRenderer::getInstance().getWindowManager()->shouldClose() || closeAsked;
}

World* Game::getWorld()
{
	return world;
}

Entity* Game::getPlayer()
{
	return player;
}

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}
