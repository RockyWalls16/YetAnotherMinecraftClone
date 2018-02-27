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

Entity::Entity(World& world) : entityWorld(world), hitbox(AABB(glm::vec3(0.1F, 0.0F, 0.1F), glm::vec3(0.9F, 1.8F, 0.9F)))
{
	entityWorld.addEntity(this);
}

void Entity::tick()
{
	shared_ptr<AirChunk> chunk = entityWorld.getChunkAtBlockPos(position.x, position.y, position.z);
	if (chunk == nullptr || (chunk != nullptr && !chunk->isGenerated()))
	{
		return;
	}

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
	AABB expandedBox = hitbox.expandBox(totalVelocity);
	std::vector<BlockAABB> blocksAABB;
	expandedBox.blockInfoInBox(entityWorld, &blocksAABB);

	for (BlockAABB& bAABB : blocksAABB)
	{
		bAABB.aabb.clipX(hitbox, &totalVelocity.x);
	}
	hitbox.move(glm::vec3(totalVelocity.x, 0, 0));

	// Reset X velocity on contact
	if (totalVelocity.x != originalVelocity.x)
	{
		velocity.x = 0.0F;
	}

	for (BlockAABB& bAABB : blocksAABB)
	{
		bAABB.aabb.clipZ(hitbox, &totalVelocity.z);
	}
	hitbox.move(glm::vec3(0, 0, totalVelocity.z));

	// Reset Z velocity on contact
	if (totalVelocity.z != originalVelocity.z)
	{
		velocity.z = 0.0F;
	}

	for (BlockAABB& bAABB : blocksAABB)
	{
		bAABB.aabb.clipY(hitbox, &totalVelocity.y);
	}
	hitbox.move(glm::vec3(0, totalVelocity.y, 0));

	// Reset Y velocity on contact
	if (totalVelocity.y != originalVelocity.y)
	{
		velocity.y = 0.0F;
	}
	// Set on ground flag
	onGround = originalVelocity.y != totalVelocity.y && originalVelocity.y < 0.0F;

	position += totalVelocity;

	// Update velocities
	controllerVelocity = glm::vec3(0.0F, 0.0F, 0.0F);
	velocity = glm::vec3(velocity.x * 0.8F, velocity.y, velocity.z * 0.8F);

	// Update hitbox position
	hitbox.updatePosCenter(position);
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

	hitbox.updatePosCenter(position);
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
	return 1.65F;
}

const glm::vec3& Entity::getVelocity() const
{
	return velocity;
}
