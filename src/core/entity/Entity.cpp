/*
 * Entity.cpp
 *
 *  Created on: 6 sept. 2017
 *      Author: Valentin
 */

#include <core/entity/Entity.h>
#include <core/world/World.h>

#include <util/Logger.h>

#define GRAVITY 9.81F / 30.0F;
#define BASE_SPEED 0.6F;

Entity::Entity(World& world) : entityWorld(world)
{
	entityWorld.addEntity(this);
	hitbox = new AABB(glm::vec3(0.1F, 0.0F, 0.1F), glm::vec3(0.9F, 1.8F, 0.9F));
}


void Entity::tick()
{
	lastPosition = position;
	lastRotation = rotation;

	// Add gravity
	if (velocity.y > -2000)
	{
		velocity.y -= GRAVITY;
	}

	glm::vec3 originalVelocity = velocity + controllerVelocity;
	glm::vec3 totalVelocity = originalVelocity;

	// Get all block surrounding player direction
	AABB* expandedBox = hitbox->expandBox(totalVelocity);
	std::vector<AABB*>* blocksAABB = expandedBox->tilesInBox(&entityWorld);

	for (AABB* bAABB : *blocksAABB)
	{
		bAABB->clipX(hitbox, &totalVelocity.x);
	}
	hitbox->move(glm::vec3(totalVelocity.x, 0, 0));

	for (AABB* bAABB : *blocksAABB)
	{
		bAABB->clipZ(hitbox, &totalVelocity.z);
	}
	hitbox->move(glm::vec3(0, 0, totalVelocity.z));

	for (AABB* bAABB : *blocksAABB)
	{
		bAABB->clipY(hitbox, &totalVelocity.y);
	}
	hitbox->move(glm::vec3(0, totalVelocity.y, 0));

	// Set on ground flag
	onGround = originalVelocity.y != totalVelocity.y && originalVelocity.y < 0.0F;

	if (onGround)
	{
		velocity.y = 0.0F;
	}

	position += totalVelocity;
	
	// Free memory
	delete(expandedBox);
	for (AABB* bAABB : *blocksAABB)
	{
		delete(bAABB);
	}
	delete(blocksAABB);

	// Update velocities
	controllerVelocity = glm::vec3(0.0F, 0.0F, 0.0F);
	velocity = glm::vec3(velocity.x * 0.8F, velocity.y, velocity.z * 0.8F);

	// Update hitbox position
	hitbox->updatePosCenter(position);
}

const glm::vec3& Entity::getControllerVelocity() const
{
	return controllerVelocity;
}

void Entity::setControllerVelocity(const glm::vec3& controllerVelocity)
{
	this->controllerVelocity = glm::vec3(controllerVelocity.x, 0.0F, controllerVelocity.z) * BASE_SPEED;

	// Jump
	if (controllerVelocity.y > 0.0F && onGround)
	{
		onGround = false;
		velocity.y += 1.5F;
	}
}

const glm::vec3& Entity::getLastPosition() const
{
	return lastPosition;
}

const glm::vec3& Entity::getLastRotation() const
{
	return lastRotation;
}

const glm::vec3& Entity::getPosition() const
{
	return position;
}

const glm::vec3& Entity::getRotation() const
{
	return rotation;
}

World& Entity::getWorld()
{
	return entityWorld;
}

void Entity::setPosition(const glm::vec3& position)
{
	this->position = position;
	this->lastPosition = position;

	hitbox->updatePosCenter(position);
}

void Entity::setRotation(const glm::vec3& rotation, bool updateLast)
{
	this->rotation = rotation;

	if(updateLast)
	{
		lastRotation = rotation;
	}
}

float Entity::getEyeHeight()
{
	return 1.75F;
}

const glm::vec3& Entity::getVelocity() const
{
	return velocity;
}
