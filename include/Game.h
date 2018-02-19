/*
 * Game.h
 *
 *  Created on: 29 ao�t 2017
 *      Author: Valentin
 */

#ifndef GAME_H_
#define GAME_H_

class Entity;
class World;

class Game
{
private:
	World* world;
	Entity* player;
	bool closeAsked;

public:
	int initGame();
	void launchGame();
	void clearGame();

	void closeGame();

	static Game& getInstance();
	World* getWorld();
	Entity* getPlayer();

	bool shouldExit();
};

#endif /* GAME_H_ */
