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
		entityWorld.keepAreaAlive(position.x, position.y, position.z, 10);
	}
}
