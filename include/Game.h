/*
 * Game.h
 *
 *  Created on: 29 août 2017
 *      Author: Valentin
 */

#ifndef GAME_H_
#define GAME_H_

class EntityPlayer;
class World;

class Game
{
private:
	World* world;
	EntityPlayer* player;
	bool closeAsked;

public:
	int initGame();
	void launchGame();
	void clearGame();

	void closeGame();

	static Game& getInstance();
	World* getWorld();
	EntityPlayer* getPlayer();

	bool shouldExit();
};

#endif /* GAME_H_ */
