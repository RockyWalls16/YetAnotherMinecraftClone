#include <core/entity/EntityPlayer.h>
#include <core/world/World.h>
#include <util/TimeManager.h>
#include <util/GameSettings.h>

EntityPlayer::EntityPlayer(World & world) : Entity(world)
{
}

void EntityPlayer::tick()
{
	Entity::tick();

	if (TimeManager::isMajorTick())
	{
		entityWorld.keepAreaAlive(*this, (int) position.x, (int) position.y, (int) position.z, GameSettings::renderDistance);
	}
}
