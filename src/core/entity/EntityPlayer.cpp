#include <core/entity/EntityPlayer.h>
#include <core/world/World.h>
#include <util/TimeManager.h>

EntityPlayer::EntityPlayer(World & world) : Entity(world)
{
}

void EntityPlayer::tick()
{
	Entity::tick();

	if (TimeManager::isMajorTick())
	{
		entityWorld.keepAreaAlive((int) position.x, (int) position.y, (int) position.z, 6);
	}
}
