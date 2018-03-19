/*
 * Entity.h
 *
 *  Created on: 6 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_ENTITY_ENTITY_H_
#define CORE_ENTITY_ENTITY_H_

#include <glm/vec3.hpp>
#include <math/AABB.h>
#include <memory>
#include <core/world/AirChunk.h>

class World;

class Entity
{
protected:
	glm::vec3 position;
	glm::vec3 lastPosition;
	glm::vec3 rotation;
	glm::vec3 lastRotation;

	glm::vec3 velocity;
	glm::vec3 controllerVelocity;

	AABB hitbox;
	bool onGround;

	World& entityWorld;
	shared_ptr<AirChunk> currentChunk;

public:
	Entity(World& world);
	virtual void tick();

	const glm::vec3& getControllerVelocity() const;
	void setControllerVelocity(const glm::vec3& controllerVelocity);
	const glm::vec3& getLastPosition() const;
	const glm::vec3& getLastRotation() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getVelocity() const;
	World& getWorld();
	shared_ptr<AirChunk> getCurrentChunk();
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation, bool updateLast = false);
	float getEyeHeight();
};

#endif /* CORE_ENTITY_ENTITY_H_ */
