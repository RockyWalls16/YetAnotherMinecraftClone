#include <core/entity/EntityPlayer.h>
#include <core/world/World.h>

EntityPlayer::EntityPlayer(World & world) : Entity(world)
{
}

void EntityPlayer::tick()
{
	Entity::tick();
	entityWorld.keepAreaAlive(position.x, position.y, position.z, 3);
}
