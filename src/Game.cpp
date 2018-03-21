/*
 * Game.cpp
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#include <Game.h>
#include <core/world/World.h>
#include <core/entity/Entity.h>
#include <core/entity/EntityPlayer.h>
#include <client/render/GameRenderer.h>
#include <client/input/GameController.h>
#include <client/audio/AudioManager.h>
#include <client/audio/AudioFile.h>
#include <util/TimeManager.h>
#include <math/MathUtil.h>
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
		player->setPosition(glm::vec3(20.0F, 35.0F, 20.0F));

		//Try to initialize game renderer
		int rendererStatus = GameRenderer::getInstance().initGameRenderer();
		if (rendererStatus)
		{
			return rendererStatus;
		}

		GameRenderer::getInstance().getGameCamera().setTrackedEntity(*player);
		AudioManager::getInstance().start();

		AudioManager::getInstance().playSound(AudioFile("music-" + std::to_string(MathUtil::rand(0, 12)) + ".ogg"));

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

		while(TimeManager::shallTick())
		{
			//Update game logic
			world->tick();
		}

		GameRenderer::getInstance().renderGame();

		/* Poll for and process events */
		glfwPollEvents();

		GameController::getInstance().processInput();
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
	AudioManager::getInstance().stop();
}

bool Game::shouldExit()
{
	return GameRenderer::getInstance().getWindowManager().shouldClose() || closeAsked;
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
