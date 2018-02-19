#ifndef CORE_ENTITY_ENTITYPLAYER_H_
#define CORE_ENTITY_ENTITYPLAYER_H_

#include <core/entity/Entity.h>

class EntityPlayer : public Entity
{
public:
	EntityPlayer(World& world);
	virtual void tick() override;
};

#endif