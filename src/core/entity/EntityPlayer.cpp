#include <core/entity/EntityPlayer.h>
#include <core/world/World.h>
#include <util/TimeManager.h>
#include <util/GameSettings.h>
#include <core/block/Block.h>

EntityPlayer::EntityPlayer(World & world) : Entity(world), cursor(0)
{
	for (int i = 0; i < 9; i++)
	{
		inventory[i] = Block::AIR;
	}
}

void EntityPlayer::tick()
{
	Entity::tick();

	if (TimeManager::isMajorTick())
	{
		entityWorld.keepAreaAlive(*this, (int) position.x, (int) position.y, (int) position.z, GameSettings::renderDistance);
	}
}

Block ** EntityPlayer::getInventory()
{
	return inventory;
}

int EntityPlayer::getCursor()
{
	return cursor;
}

void EntityPlayer::setCursor(int newCursor)
{
	cursor = newCursor;
}
