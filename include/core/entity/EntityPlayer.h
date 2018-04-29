#ifndef CORE_ENTITY_ENTITYPLAYER_H_
#define CORE_ENTITY_ENTITYPLAYER_H_

#include <core/entity/Entity.h>

class Block;

class EntityPlayer : public Entity
{
private:
	Block * inventory[9];
	int cursor;

public:
	EntityPlayer(World& world);
	virtual void tick() override;

	Block** getInventory();
	int getCursor();
	void setCursor(int newCursor);
};

#endif